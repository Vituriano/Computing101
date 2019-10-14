import os, pyxhook
import serial

# TODO: saida serial p/o arduino

# Mapeamento de Teclas para comandos que serão enviados p/o Arduino
key_dict = {"Left": "l", "Right":'r',
        "a": 'l', "s": 'r',
        "space": 's'}

# Nenhum desses valores valem nada. São só placeholders
port = "/dev/ttyACM0"
ard_serial = serial.Serial(port, 9600)

# Transformar a String da tecla em um comando entendivel pelo Arduino
def EncodeKey(key):
    if key in key_dict:
        return key_dict[key]

def SendData(key):
    encoded = EncodeKey(key)
    if encoded != None:
        print("Sending: {}".format(encoded))
        # TODO: não sei se isso funciona
        if ard_serial.isOpen():
            b = bytearray()
            b.extend(map(ord, encoded))
            ard_serial.write(b)
        else:
            print("Serial não está disponivel :(")

# Função que será chamada quando uma tecla for pressionada
def OnKeyPress(event):
    print("Key: {}".format(event.Key))
    SendData(event.Key)

# Conectar o hook
hook = pyxhook.HookManager()
hook.KeyDown = OnKeyPress
hook.HookKeyboard()

# Iniciar o hook num try catch except
try:
    hook.start()
except KeyboardInterrupt:
    pass
