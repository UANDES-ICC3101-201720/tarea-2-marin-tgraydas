/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
char *physmem;
int pages_lefts = 0;
int diskwrite = 0;
int diskread = 1;
int npages;
int nframes;
int *marcos_table;
int removepos = 1;
char *algorithm;
struct disk *disk;
struct node
{
	int value;
	int page;
	struct node *next;
};
struct list
{
	struct node *node;
};

struct list *head;
void append(int value, int page, struct node *initial, struct node *next)
{
	next = initial;
	if (next != 0)
	{
		while (next->next != 0)
		{
			next = next->next;
		}
	}
	next->next = malloc(sizeof(struct node));
	next = next->next;
	next->value = value;
	next->page = page;
}

struct node *pop(struct node *initial)
{
	struct node *next = initial;
	struct node *before;
	while (next->next != NULL)
	{
		before = next;
		next = next->next;
	}
	before->next = NULL;
	return next;
}

void remove_first()
{
	head->node = head->node->next;
}

int findPageByFrame(int frame)
{
	struct node *next = head->node;
	while (next != NULL)
	{
		if (next->value == frame)
		{
			return next->page;
		}
		next = next->next;
	}
	return -1;
}

void modify(int frame, int page)
{
	struct node *next = head->node;
	while (next != NULL)
	{
		if (next->value == frame)
		{
			next->page = page;
			return;
		}
		next = next->next;
	}
}

void HALFREMOVER(struct page_table *pt, int page)
{
	removepos++;
	if (removepos == nframes)
	{
		removepos = 0;
	}
	int frame = removepos;
	int the_page = findPageByFrame(frame);
	modify(frame, page);
	disk_write(disk, the_page, &physmem[frame * PAGE_SIZE]);
	diskwrite++;
	disk_read(disk, page, &physmem[frame * PAGE_SIZE]);
	diskread++;
	page_table_set_entry(pt, the_page, frame, 0);
	page_table_set_entry(pt, page, frame, PROT_READ);
}

void FIFO(struct page_table *pt, int page)
{
	int frame = head->node->value;
	int the_page = head->node->page;
	remove_first(head);
	struct node *next = head->node;
	append(frame, page, head->node, next);
	disk_write(disk, the_page, &physmem[frame * PAGE_SIZE]);
	diskwrite++;
	disk_read(disk, page, &physmem[frame * PAGE_SIZE]);
	diskread++;
	page_table_set_entry(pt, the_page, frame, 0);
	page_table_set_entry(pt, page, frame, PROT_READ);
}

void RAND(struct page_table *pt, int page)
{
	int frame = lrand48() % nframes;
	int the_page = findPageByFrame(frame);
	disk_write(disk, the_page, &physmem[frame * PAGE_SIZE]);
	diskwrite++;
	disk_read(disk, page, &physmem[frame * PAGE_SIZE]);
	diskread++;
	modify(frame, page);
	page_table_set_entry(pt, the_page, frame, 0);
	page_table_set_entry(pt, page, frame, PROT_READ);
}

struct node *loadedPage(int page)
{
	struct node *node = head->node;
	while (node != NULL)
	{
		if (node->page == page)
		{
			return node;
		}
		node = node->next;
	}
	return NULL;
}

void page_fault_handler(struct page_table *pt, int page)
{
	pages_lefts++;
	struct node *loaded = loadedPage(page);
	if (loaded != NULL)
	{
		page_table_set_entry(pt, page, loaded->value, PROT_READ | PROT_WRITE);
		return;
	}
	int helper = 0;
	for (int i = 0; i < nframes; i++)
	{
		if (marcos_table[i] != -1)
		{
			disk_read(disk, page, &physmem[i * PAGE_SIZE]);
			diskread++;
			page_table_set_entry(pt, page, i, PROT_READ);
			marcos_table[i] = -1;
			struct node *next = head->node;
			if (i == 0)
			{
				head->node->value = 0;
				head->node->page = page;
				helper = 1;
				break;
			}
			append(i, page, head->node, next);
			helper = 1;
			break;
		}
	}

	if (!helper)
	{
		if (!strcmp(algorithm, "fifo"))
		{
			FIFO(pt, page);
		}
		else if (!strcmp(algorithm, "custom"))
		{

			HALFREMOVER(pt, page);
		}
		else if (!strcmp(algorithm, "random"))
		{
			RAND(pt, page);
		}
	}
}

int main(int argc, char *argv[])
{
	head = malloc(sizeof(struct list));
	head->node = malloc(sizeof(struct node));
	if (argc != 5)
	{
		/* Add 'random' replacement algorithm if the size of your group is 3 */
		printf("use: virtmem <npages> <nframes> <random|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	npages = atoi(argv[1]);
	nframes = atoi(argv[2]);

	algorithm = argv[3];

	int m[nframes];
	marcos_table = m;
	for (int i = 0; i < nframes; ++i)
	{
		marcos_table[i] = i;
	}
	//const char *algorithm = argv[3];
	const char *program = argv[4];

	disk = disk_open("myvirtualdisk", npages);
	if (!disk)
	{
		fprintf(stderr, "couldn't create virtual disk: %s\n", strerror(errno));
		return 1;
	}

	struct page_table *pt = page_table_create(npages, nframes, page_fault_handler);

	if (!pt)
	{
		fprintf(stderr, "couldn't create page table: %s\n", strerror(errno));
		return 1;
	}

	char *virtmem = page_table_get_virtmem(pt);
	physmem = page_table_get_physmem(pt);
	for (int i = 0; i < nframes; ++i)
	{
		disk_write(disk, i, &physmem[i * BLOCK_SIZE]);
	}

	if (!strcmp(program, "sort"))
	{
		sort_program(virtmem, npages * PAGE_SIZE);
	}
	else if (!strcmp(program, "scan"))
	{
		scan_program(virtmem, npages * PAGE_SIZE);
	}
	else if (!strcmp(program, "focus"))
	{
		focus_program(virtmem, npages * PAGE_SIZE);
	}
	else
	{
		fprintf(stderr, "unknown program: %s\n", argv[4]);
	}
	/*printf("Faltas de pagina: %i\n", pages_lefts);
	printf("Lecturas de disco: %i\n", diskread);
	printf("Escrituras de disco: %i\n", diskwrite);*/
	printf("%i,%i,%i,%i\n", pages_lefts, nframes, diskwrite, diskread);
	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
