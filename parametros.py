from subprocess import PIPE, run

import os
for r1 in [1, 3, 5]:
    for r2 in [1, 3, 5]:
        for numCoiotes in [3, 5, 7]:
            for numMatilhas in [2, 3, 5]:
                somaArquivos=0
                cont=0
                for file in os.listdir("test/P"):
                    if file.endswith(".vrp"):
                        command = ["./prog", "test/P/"+str(file), str(r1), str(r2), "100", str(numCoiotes), str(numMatilhas)]
                        somaResultado = 0
                        for _ in range(5):
                            result = run(command, stdout=PIPE, stderr=PIPE, universal_newlines=True)
                            valorResultado = int(result.stdout)
                            somaResultado+= valorResultado
                        media = somaResultado/5.0
                        arq, ext = os.path.splitext(file)
                        otimo = -1
                        with open('test/P/%s.sol'%(arq), 'r') as f:
                            lines = f.read().splitlines()
                            last_line = lines[-1]
                            partes = last_line.split(' ')
                            otimo = float(partes[-1])
                        if otimo == -1:
                            raise 'Error'
                        cont+=1
                        somaArquivos += (otimo/media)*100
                mediaParametros = somaArquivos/cont
                with open('param_test/%s-%s-%s-%s.out'%(str(r1), str(r2), str(numCoiotes), str(numMatilhas)), 'w') as file:
                    file.write('Parâmetros r1=%s, r2=%s, num_coiotes=%s, num_matilhas=%s'%(str(r1), str(r2), str(numCoiotes), str(numMatilhas)))
                    file.write('Media=%s'%(str(mediaParametros)))
