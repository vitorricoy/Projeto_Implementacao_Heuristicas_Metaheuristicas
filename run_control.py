from subprocess import PIPE, run
from timeit import default_timer as timer
import os
somaArquivos=0
cont=0
for dir in os.listdir("test"):
    if dir != 'P':
        for file in os.listdir("test/"+dir):
            if file.endswith(".vrp"):
                arq, ext = os.path.splitext(file)
                results = []
                times = []
                for i in range(5):
                    command = ["controle/Program/genvrp", "test/"+dir+"/"+str(file), "control_results/"+dir+"/"+arq+"_"+str(i)+".sol", "-it", "500"]
                    start = timer()
                    result = run(command, stdout=PIPE, stderr=PIPE, universal_newlines=True)
                    end = timer()
                    with open("control_results/"+dir+"/"+arq+"_"+str(i)+".sol", "a") as myfile:
                        myfile.write("Tempo = %s"%(str(end-start)))