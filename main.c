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


void page_fault_handler( struct page_table *pt, int page)
{
	printf("page fault on page #%d\n",page);
	exit(0);

	}
	

int main( int argc, char *argv[] )
{
	if(argc!=5) {
		/* Add 'random' replacement algorithm if the size of your group is 3 */
		printf("use: virtmem <npages> <nframes> <lru|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	int  marcos_table_libres[nframes];
	int  marcos_table[nframes];
	for (int i = 0; i < nframes; ++i)
	{
		marcos_table[i] = i;
	}
	const char * algorithm = argv[3];
	const char *program = argv[4];

	struct disk *disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}



	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );

	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}

	char *virtmem = page_table_get_virtmem(pt);

	char *physmem = page_table_get_physmem(pt);
	int  *frame;
	int  * bits;
	for (int paginas = 0; paginas < npages ; ++paginas)
	{
		if(!strcmp(algorithm,"lru")) {
			//implementar lru
			
		int a_disco = 0;
		if (npages > nframes)
		{
			int i = 0;
			while(marcos_table_libres[i]==-1 && i <= marcos_table[i])
			{

				if (i == nframes)
				{
					a_disco = 1;
					break;

				}
				i++;
			}
			if (a_disco== 0 && marcos_table_libres[i] != -1  && i <= marcos_table[i] )
			{
				printf(" holaa %d\n",marcos_table_libres[i]  );
				page_table_get_entry(pt,paginas, frame, bits);
				int frames = frame;
				int bit  = bits;
				page_table_set_entry(pt, paginas, frames,bit);
				marcos_table_libres[i] = -1;
				printf(" holaa %d\n",marcos_table_libres[i]  );


			}else{
				printf(" hola como %d\n",marcos_table_libres[i]  );
				//manejar disco
			}

		}
		
	}
	if(npages ==  nframes){
		for (int i = 0; i < nframes; ++i)
		{
			page_table_set_entry(pt, i, i,PROT_WRITE);
		}

		

} else if(!strcmp(algorithm,"fifo")) {
		// implementar fifo
		scan_program(virtmem,npages*PAGE_SIZE);

} else if(!strcmp(algorithm,"custom")) {
		//implementar custom
		focus_program(virtmem,npages*PAGE_SIZE);

} else {
		fprintf(stderr,"unknown algorithm: %s\n",argv[3]);

	}
}






if(!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);

}else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

} else {
		fprintf(stderr,"unknown program: %s\n",argv[4]);

	}
	


	

	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
