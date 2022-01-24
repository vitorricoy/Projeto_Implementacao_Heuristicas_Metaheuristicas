from subprocess import PIPE, run

import os
cont = 0
for r1 in [0.2, 0.5, 0.75, 1]:
    for r2 in [0.2, 0.5, 0.75, 1]:
        numCoiotes = 5
        numMatilhas = 10
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
                somaArquivos += (media/otimo)
        mediaParametros = somaArquivos/cont
        with open('param_test/%s.out'%str(cont), 'w') as file:
            file.write('Parâmetros r1=%s, r2=%s, num_coiotes=%s, num_matilhas=%s\n'%(str(r1), str(r2), str(numCoiotes), str(numMatilhas)))
            file.write('Media=%s'%(str(mediaParametros)))
        cont+=1
