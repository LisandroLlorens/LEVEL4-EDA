####################################
Nombre del Grupo: MightyATP(adenosin Trifosfato)
Integrantes: Juan Martin Rodriguez Arias, Lisandro Llorens
####################################



# Nuestro Algoritmo
A grandes rasgos, nuestro algoritmo tiene dos pasos: la función traverse y la función floodFill.

## floodFill
Nuestra implementacion del algoritmo floodFill funciona utilizando las paredes que el ratón descubre. Utilizamos una queue al igual que una marca para recorrer y analizar las celulas del laberinto como si fuera un grafo. Esto nos permite elegir casillas objetivo y recorrer sus vecinos. Si no tienen una pared que le obstruya el paso al raton (y si no fueron recorridas previamente) entonces se les asigna un valor de distancia igual a uno mas el valor de distancia que tenia la celula anterior.

## traverse
Esta función utiliza el algoritmo floodfill para avanzar siempre intentando reducir su distancia. Este prefiere seguir recto en vez de doblar dada la igualdad de distancia para minimizar giros.

Si en cualquier momento se encuentra "encerrado" entre paredes y/o valores mayores de distancia que el que tiene la celula en la que esta parada el raton, se llama nuevamente el algoritmo floodFill para recalcular con las nuevas paredes que encontró el ratón.

## Estrategia
Nuestra estrategia principal consiste en utilizar esta intercalacion entre floodFill y traverse para ir y volver del origen al objetivo un total de 5 veces (de tal forma que la ultima vez sea la 3er ida).

Para esto, utilizamos dos "modos" GO y RETURN. Estos modos definen si el raton esta yendo al objetivo o volviendo.

Esta estrategia nos permite utilizar los regresos para encontrar más paredes y explorar mas caminos que potencialmente sean optimos. A pesar de que puede desviarse por ramas en algunos casos, esta estrategia suele encontrar el camino mas optimo al llegar a la tercer ida.