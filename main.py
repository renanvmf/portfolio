import sqlite3

conn = sqlite3.connect('simplesystem.db')
c = conn.cursor()

#CONFERINDO SE A TABELA JÁ EXISTE. SE NÃO, EXECUTA A CRIAÇÃO DA MESMA
# get the count of tables with the name
c.execute(''' SELECT count(name) FROM sqlite_master WHERE type='table' AND name='user' ''')
# if the count is 1, then table exists.
if c.fetchone()[0] == 1: {
    print('Table "user" exists.')
}
else:
    c.execute('''CREATE TABLE "user" ("user_number"	INTEGER NOT NULL UNIQUE, "login" TEXT NOT NULL UNIQUE, "password" TEXT NOT NULL, "nome_usuario" TEXT NOT NULL UNIQUE,
 "cpf" INTEGER NOT NULL UNIQUE, PRIMARY KEY("user_number"));''')

def ler_tabela():
    c.execute(''' SELECT * FROM user ''')
    print(c.fetchall())


def registro():
    login = ''
    while login != '/q':
        #Conferindo e configurando o número do usuário (Chave primária)

        c.execute('SELECT MAX(user_number) FROM user')
        pk = c.fetchone()
        if pk[0] == None:
            pk = 1
        else:
            pk = pk[0]
            pk = pk + 1
        #Adicionando o login e conferindo se o mesmo já consta no banco de dados.
        ct = True
        while ct == True:
            print("DIGITE O LOGIN: \n (Em qualquer momento, digite /q para sair do programa.)\n")
            login = str(input()).upper()
            if login == '':
                print('Por favor, insira um usuário.')
            c.execute('''SELECT * FROM user WHERE login LIKE (?) ''',[login])
            if c.fetchone() != None:
                print('O usuário não está disponível')
            else:
                ct = False
        if login == '/Q':
            break
        #Requisitando a senha
        ps = False
        while ps == False:
            print("DIGITE A SENHA: \n")
            senha = str(input()).upper()
            if senha == '/Q' or senha == '/q':
                break
            elif senha == "" or len(senha) < 6:
                print("Favor não deixar em branco, digite no mínimo 6 digitos para a senha")

            else:
                ps = True
        if ps == False:
            break

        #Requisitando nome_usuario
        print("Digite o nome do usuário: \n")
        nome = str(input()).upper()
        if nome == '/Q':
            break

        #Requisitando cpf
        print("Digite o cpf (somente numeros): \n")
        p = False
        while p == False:
            try:
                cpf = input()
                if cpf == '/q':
                    break
                cpf = int(cpf)
                scpf = str(cpf)

            except:
                print("Favor dar entrada apenas com números")
                p2 = 1
                scpf = '0'
                cpf = 0
            if len(scpf) != 11 and cpf != 0:
                print('Favor entrar com os 11 digitos do cpf')
            elif len(scpf) == 11:
                p = True

        if p == False or cpf == 0 or scpf == '0' :
            break
        insert_data = [pk,login,senha,nome,cpf]
        c.execute('INSERT INTO user VALUES (?,?,?,?,?)',insert_data)
        conn.commit()




registro()
ler_tabela()




