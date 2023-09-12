def encrypt(text, shift):
    encrypted_text = ""
    
    for char in text:
        if char.isalpha():
            is_upper = char.isupper()
            char_index = ord(char.lower()) - ord('a')
            shifted_index = (char_index + shift) % 26
            shifted_char = chr(shifted_index + ord('a'))
            if is_upper:
                shifted_char = shifted_char.upper()
            encrypted_text += shifted_char
        else:
            encrypted_text += char
    
    return encrypted_text

def decrypt(encrypted_text, shift):
    return encrypt(encrypted_text, -shift)

plaintext = input("Masukkan pesan yang akan dienkripsi :\n")
shift = int(input("Masukkan key : "))
encrypted_text = encrypt(plaintext, shift)
print("Teks Enkripsi:", encrypted_text)

decrypted_text = decrypt(encrypted_text, shift)
print("Teks Dekripsi:", decrypted_text)