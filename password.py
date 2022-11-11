#recebe 2 palavras
 #   inverte as palavras
  #  cipher rot47
   #     (pega em cada palavra splits em char e avanca os chars na tabela de ascii)
    #    junta as duas palavras em 1 unica (para ficar maior)
    #print palavra

palavra = ""  # Colocar a palavra desejada

def encode_rot47(s):
    x = []
    for i in range(len(s)):
        j = ord(s[i])
        if j >= 33 and j <= 126:
            x.append(chr(33 + ((j + 14) % 94)))
        else:
            x.append(s[i])
    return ''.join(x)



def decode_rot47(data):
        decode = []
        for i in range(len(data)):
                encoded = ord(data[i]) 
                if encoded >= 33 and encoded <= 126:
                        decode.append(chr(33 + ((encoded + 14) % 94)))
                else:
                        decode.append(data[i])
        return ''.join(decode)

print(encode_rot47(palavra))
print(decode_rot47(encode_rot47(palavra)))



# Poder inserir que simbolos nao quero que aparecam e ele sobe na tabela de asci esses simbolos
