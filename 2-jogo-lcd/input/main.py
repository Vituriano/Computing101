import os, pyxhook
import serial

# TODO: saida serial p/o arduino

# Mapeamento de Teclas para comandos que serão enviados p/o Arduino
key_dict = {"Left": -1, "Right": 1,
        "a": -1, "d": 1,
        "space": 0}

# Nenhum desses valores valem nada. São só placeholders
port = "/dev/algumacoisa"
#ard_serial = serial.Serial(port, 9600,timeout=2)

# Transformar a String da tecla em um comando entendivel pelo Arduino
def EncodeKey(key):
    if key in key_dict:
        return key_dict[key]

def SendData(key):
    encoded = EncodeKey(key)
    if encoded != None:
        print("Sending: {}".format(encoded))
        # TODO: não sei se isso funciona
        #ard_serial.write(str(encoded))

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
