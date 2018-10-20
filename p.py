from matplotlib import pyplot as plt
lista = ["fifo-focus.txt", "fifo-scan.txt", "fifo-sort.txt", "random-sort.txt",
         "random-scan.txt", "random-focus.txt", "custom-sort.txt", "custom-scan.txt", "custom-focus.txt"]

data = []
for filename in lista:
    file = open(filename, "r")
    file_data = []
    for linea in file:
        file_data.append(linea.strip().split(","))
    file.close()
    data.append(file_data)
pos = 0
for i in data:
    marcos = []
    faltas = []
    for x in i:
        marcos.append(int(x[1]))
        faltas.append(int(x[0]))
    plt.xlabel("Marcos")
    plt.ylabel("Faltas de Pagina")
    plt.plot(marcos, faltas)
    plt.title((lista[pos].split("-")[0] + " " +
               lista[pos].split("-")[1].split(".")[0]).upper())
    plt.show()
    marcos = []
    faltas = []
    for x in i:
        marcos.append(int(x[1]))
        faltas.append(int(x[2]))
    plt.xlabel("Marcos")
    plt.ylabel("Escrituras de Disco")
    plt.plot(marcos, faltas)
    plt.title((lista[pos].split("-")[0] + " " +
               lista[pos].split("-")[1].split(".")[0]).upper())
    plt.show()
    marcos = []
    faltas = []
    for x in i:
        marcos.append(int(x[1]))
        faltas.append(int(x[3]))
    plt.xlabel("Marcos")
    plt.ylabel("Lecturas de Disco")
    plt.plot(marcos, faltas)
    plt.title((lista[pos].split("-")[0] + " " +
               lista[pos].split("-")[1].split(".")[0]).upper())
    plt.show()
    pos += 1
