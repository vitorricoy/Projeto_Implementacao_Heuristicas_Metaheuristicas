from subprocess import PIPE, run
from timeit import default_timer as timer
import os
somaArquivos=0
cont=0
for dir in os.listdir("test"):
    if dir not in ['DIMACS', 'P']:
        for file in os.listdir("test/"+dir):
            if file.endswith(".vrp"):
                arq, ext = os.path.splitext(file)
                n = int(arq.split('-')[1][1:])
                if n >= 40:
                    continue
                results = []
                times = []
                for i in range(10):
                    command = ["./prog", "test/"+dir+"/"+str(file), "1", "0.75", "1000", "5", "10"]
                    start = timer()
                    result = run(command, stdout=PIPE, stderr=PIPE, universal_newlines=True)
                    end = timer()
                    results.append(result.stdout)
                    times.append(end-start)
                print('Escrito', file)
                with open('test_results_1000/'+str(dir)+'/'+arq+'.sol', 'w') as file:
                    file.write('Resultados = '+str(results))
                    file.write('\nTempos = '+str(times))
                