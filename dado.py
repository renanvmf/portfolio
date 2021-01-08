import random

rolar = ""
dado = [1,2,3,4,5,6]
while rolar != "N":

    rolar = str(input("Você deseja rolar o dado? (S/N)\n")).upper()
    if rolar == "S":
        lado = random.choice(dado)
        print("O número sorteado foi " + str(lado) + "\n")
    elif rolar == "N":
        print("O programa será fechado.\n")
        break
    else:
        print("Favor entrar apenas com S ou N\n")