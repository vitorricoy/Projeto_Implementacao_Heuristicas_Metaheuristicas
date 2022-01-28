from subprocess import PIPE, run
from timeit import default_timer as timer
import os
import random
somaArquivos=0
cont=0
for dir in os.listdir("test"):
    if dir != 'P' and dir != 'DIMACS':
        for file in os.listdir("test/"+dir):
            if file.endswith(".vrp"):
                arq, ext = os.path.splitext(file)
                results = []
                times = []
                for i in range(5):
                    command = ["controle/Program/genvrp", "test/"+dir+"/"+str(file), "control_results/"+dir+"/lixo/"+arq+"_"+str(i)+".sol", "-it", "100", "-seed", str(random.randint(0, 100000001))]
                    start = timer()
                    result = run(command, stdout=PIPE, stderr=PIPE, universal_newlines=True)
                    end = timer()
                    with open("control_results/"+dir+"/lixo/"+arq+"_"+str(i)+".sol", "r") as myfile:
                        lines = myfile.read().splitlines()
                        costLine = lines[-2]
                        cost = costLine.split(' ')[1]
                        results.append(cost)
                    times.append(end-start)
                print('Escrito', file)
                with open('control_results/'+str(dir)+'/'+arq+'.sol', 'w') as file:
                    file.write('Resultados = '+str(results))
                    file.write('\nTempos = '+str(times))