/**
 * This is the Vigenere class that implements Encryptor.
 * Vigenere encrypts and decrypts a message with the vigenere method.
 * @author MIDN 3/C NICHOLAS ZAYFMAN
 */
public class Vigenere implements Encryptor {
    
    //Private field to store key.
    private static char[] vigenereKey;

    /**
     * This method returns the algorithm name: vigenere.
     * @return "vigenere"
     */
    public String getAlgName(){
        
        return "vigenere";
    }

    /**
     * This method defines the private static field psswd and error
     * checks the key.
     * @param key The string that is needed to encrypt or decrypt a message.
     * @throws IndexOutOfBoundsException throws error message if
     * a certian character in the key is outside the ASCII values
     * 42 to 122. 
     */
    public void init(char[] key) throws IndexOutOfBoundsException{
        
        vigenereKey = key;
        
        //checks error in key
        for(int i = 0; i < vigenereKey.length; i++){
            if(vigenereKey[i]>122 || vigenereKey[i]<42){
                throw new IndexOutOfBoundsException("Error "
                 + vigenereKey[i] + " not allowed in key");
            }
        }
    }

    /**
     * This methods error checks the plain message then encrypts it with the 
     * vignere method.
     * @param plain The message we are going to encrypt.
     * @return The encrypted message.
     * @throws IndexOutOfBoundsException throws error message if
     * a certian character in the plaintext is outside the ASCII values
     * 42 to 122. 
     */
    public String encrypt(String plain) throws IndexOutOfBoundsException{
        
        String c = "";
        
        //checks error in plain then encrypts.
        for(int i = 0; i < plain.length(); i++){
            if(plain.charAt(i)>122 || plain.charAt(i)<42){
                throw new IndexOutOfBoundsException("Error "
                 + plain.charAt(i) + " not allowed in plaintext");
            }
            else{
                c = c + (char)(42+((int)vigenereKey[i%vigenereKey.length]
                +(int)plain.charAt(i)-84)%81);
            }
        }
        
        return c;
    }

    /**
     * This method error checks the cipher or encrypted text
     * and then decrypts it using the vigenere method.
     * @param cipher The encrypted message.
     * @return The plain message that was encrypted in the cipher.
     * @throws IndexOutOfBoundsException throws error message if
     * a certian character in the cipher is outside the ASCII values
     * 42 to 122. 
     */
    public String decrypt(String cipher) throws IndexOutOfBoundsException{
        
        String c = "";

        //check error in cipher then decrypts.
        for(int i = 0; i < cipher.length(); i++){
            if(cipher.charAt(i)<42 || cipher.charAt(i)>122){
                String error = "Error " + cipher.charAt(i) + 
                " not allowed in cybertext";
                throw new IndexOutOfBoundsException(error);   
            }
            else{ 
                c = c + (char)(42+(cipher.charAt(i)-42 + 81 - 
                vigenereKey[i%vigenereKey.length]+42)%81);
           }
        }

        return c;
    }
}