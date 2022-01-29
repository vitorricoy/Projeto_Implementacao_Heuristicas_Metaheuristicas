import os
import ast

for file in os.listdir("."):
    if file.endswith('.sol'):
        with open(file, "r") as arq:
            lines = arq.read().splitlines()
            resultados = ast.literal_eval(lines[0][13:])
            tempos = ast.literal_eval(lines[1][9:])
            resultado_medio = 0
            for r in resultados:
                resultado_medio += float(r)
            resultado_medio /= len(resultados)
            resultado_porc = resultado_medio
            with open('../../../test/B/'+str(file), 'r') as opt_file:
                lines = opt_file.read().splitlines()
                while lines[-1] == '':
                    lines.pop()
                cost = float(lines[-1].split(' ')[1])
                resultado_porc/=cost
                resultado_porc-=1
            tempo_medio = 0
            for t in tempos:
                tempo_medio += float(t)
            tempo_medio /= len(tempos)
            print(file, ":", "tempo="+str(tempo_medio), ", solucao="+str(resultado_medio), ", razão="+str(resultado_porc))
