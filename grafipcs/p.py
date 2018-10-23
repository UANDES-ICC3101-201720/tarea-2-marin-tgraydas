from matplotlib import pyplot as plt
lista = ["fifo-focus.txt", "fifo-scan.txt", "fifo-sort.txt", "random-sort.txt",
         "random-scan.txt", "random-focus.txt", "custom-sort.txt", "custom-scan.txt", "custom-focus.txt"]

data = []
for filename in lista:
    file = open("data/" + filename, "r")
    file_data = []
    for linea in file:
        file_data.append(linea.strip().split(","))
    file.close()
    data.append(file_data)
pos = 0
for i in data:
    marcos = []
    escritura = []
    faltas = []
    lecturas = []
    for x in i:
        marcos.append(int(x[1]))
        escritura.append(int(x[2]))
        lecturas.append(int(x[3]))
        faltas.append(int(x[0]))
    plt.xlabel("Marcos")
    plt.ylabel("Cantidad")
    plt.plot(marcos, faltas, "r", label="Faltas de Pagina")
    plt.plot(marcos, escritura, "b", label="Escrituras de Disco")
    plt.plot(marcos, lecturas, "g--", label="Lecturas de Disco")
    plt.title((lista[pos].split("-")[0] + " " +
               lista[pos].split("-")[1].split(".")[0]).upper())
    fig = plt.gcf()
    plt.legend()
    plt.show()
    plt.draw()
    fig.savefig(lista[pos].split(".")[0])
    pos += 1
