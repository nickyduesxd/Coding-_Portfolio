# project.py
# NICHOLAS ZAYFMAN
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad
from Cryptodome.Util.Padding import unpad
from Cryptodome.Random import random
from Cryptodome import Random 
import os

#PART6 BELOW
def part6():
  key = Random.get_random_bytes(16)
  iv = Random.get_random_bytes(16)
  encrypt_ecb("pic_original.bmp" , "pic_ecb.bin", key)
  encrypt_cbc("pic_original.bmp" , "pic_cbc.bin", key, iv)
  encrypt_ctr("pic_original.bmp" , "pic_ctr.bin", key, iv)
  
  with open("pic_ecb.bin", 'rb+') as file:
    file.seek(69) 
    file.write(b'\x00') 
  
  with open("pic_cbc.bin", 'rb+') as file:
    file.seek(69) 
    file.write(b'\x00') 

  with open("pic_ctr.bin", 'rb+') as file:
    file.seek(69) 
    file.write(b'\x00') 

  decrypt_ecb("pic_ecb.bin", "pic_ecb_corrupted.bmp", key)
  decrypt_cbc("pic_cbc.bin", "pic_cbc_corrupted.bmp", key, iv)
  decrypt_ctr("pic_ctr.bin", "pic_ctr_corrupted.bmp", key, iv)


#PART5 BELOW
# no = 4e6f0e0e0e0e0e0e0e0e0e0e0e0e0e0e
# yes = 5965730d0d0d0d0d0d0d0d0d0d0d0d0d

def break_cbc():
  ivOld = bytes.fromhex("76fe6ed7a7532ab5cf4dd01d777ba0d8")
  m = bytes.fromhex("4e6f0e0e0e0e0e0e0e0e0e0e0e0e0e0e")
  ivNew = bytes.fromhex("de2f3b35742e7ceb4b039d8ae4821cc5")

  x = bytes(a ^ b for a, b in zip(ivOld, m))
  y = bytes(a ^ b for a, b in zip(ivNew, x))

  print("message to enter")
  print(y.hex())




#PART4 BELOW

def break_ctr():
    
  plaintext = bytes.fromhex("546869732069732061206b6e6f776e206d65737361676521")
  ciphertext = bytes.fromhex("ed6b6ee550650f5b9bb699678e244260c3bc87ec8a3172d7")
  target = bytes.fromhex("d26d68e11e2c0c179bff9c7d842b5860cfad9fbfa80245fc")

  x = bytes(a ^ b for a, b in zip(plaintext, ciphertext))
  y = bytes(a ^ b for a, b in zip(x, target))
  print(y)


#PART3 BELOW

def pad2(data):
  length = 16 - len(data) % 16
  padding = bytes([length] * length)
  return data + padding


def unpad2(padded_data):

  pad_value = padded_data[-1]

  if pad_value > len(padded_data) and len(padded_data)%16 != 0:
    print("padding error!")
    return None
  
  for byte in padded_data[-pad_value:]:
    if byte != pad_value:
      print("padding error!")
      return None
      break

  return padded_data[:-pad_value]


#PART 2 BELOW 
def encrypt_ctr(in_plain_file, out_cipher_file, key, ctr):
  with open(in_plain_file, 'rb') as input_file:
    plaintext = input_file.read()
    
  nonce= ctr[:8]
  iv= ctr[8:]
  cipher = AES.new(key, AES.MODE_CTR, nonce=nonce, initial_value=iv)
  ciphertext = cipher.encrypt(plaintext)

  with open(out_cipher_file, 'wb') as output_file:
    output_file.write(ciphertext)


def decrypt_ctr(in_cipher_file, out_plain_file, key, ctr):
  with open(in_cipher_file, 'rb') as input_file:
    ciphertext = input_file.read()

  nonce= ctr[:8]
  iv= ctr[8:]

  cipher = AES.new(key, AES.MODE_CTR, nonce=nonce, initial_value=iv)
  plaintext = cipher.decrypt(ciphertext)

  with open(out_plain_file, 'wb') as output_file:
    output_file.write(plaintext)

def encrypt_cbc(in_plain_file, out_cipher_file, key, iv):
  with open(in_plain_file, 'rb') as input_file:
    plaintext = input_file.read()
    
  cipher = AES.new(key, AES.MODE_CBC, iv)
  padded_plaintext = pad(plaintext, AES.block_size)
  ciphertext = cipher.encrypt(padded_plaintext)

  with open(out_cipher_file, 'wb') as output_file:
    output_file.write(ciphertext)


def decrypt_cbc(in_cipher_file, out_plain_file, key, iv):
  with open(in_cipher_file, 'rb') as input_file:
    ciphertext = input_file.read()

  cipher = AES.new(key, AES.MODE_CBC, iv)
  decrypted_data = cipher.decrypt(ciphertext)
  plaintext = unpad(decrypted_data, AES.block_size)

  with open(out_plain_file, 'wb') as output_file:
    output_file.write(plaintext)



##PART 1 BELOW ECB
# read in_plain_file, encrypt the data, and store the ciphertext in out_cipher_file
# read in_plain_file, encrypt the data, and store the ciphertext in out_cipher_file
def encrypt_ecb(in_plain_file, out_cipher_file, key):
  with open(in_plain_file, 'rb') as input_file:
    plaintext = input_file.read()
    
  cipher = AES.new(key, AES.MODE_ECB)
  padded_plaintext = pad(plaintext, AES.block_size)
  ciphertext = cipher.encrypt(padded_plaintext)

  with open(out_cipher_file, 'wb') as output_file:
    output_file.write(ciphertext)

# read in_cipher_file, decrypt the ciphertext, and store the plaintext in out_plain_file
def decrypt_ecb(in_cipher_file, out_plain_file, key):
  
  with open(in_cipher_file, 'rb') as input_file:
    ciphertext = input_file.read()

  cipher = AES.new(key, AES.MODE_ECB)
  decrypted_data = cipher.decrypt(ciphertext)
  plaintext = unpad(decrypted_data, AES.block_size)

  with open(out_plain_file, 'wb') as output_file:
    output_file.write(plaintext)



# read normal_bmp_file and in_cipher_file, fix the header in the ciphertext and
# store the results in out_cipher_bmp_file
def fix_bmp_header(normal_bmp_file, in_cipher_file, out_cipher_bmp_file):

  with open(normal_bmp_file, 'rb') as normal_file:
    header =normal_file.read(54)
  
  with open(in_cipher_file, 'rb') as input_file:
    ciphertext = input_file.read()[54:]

  with open(out_cipher_bmp_file, 'wb') as output_file:
    output_file.write(header)
    output_file.write(ciphertext)