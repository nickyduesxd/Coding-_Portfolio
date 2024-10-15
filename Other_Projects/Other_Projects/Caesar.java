/**
 * This is the Caesar class that implements Encryptor.
 * Caesar encrypts and decrypts a message with the caesar method.
 * @author MIDN 3/C NICHOLAS ZAYFMAN
 */
public class Caesar implements Encryptor {
    
    //Private Field
    private static int caesarKey;

    /**
     * This method returns the algorithm name: caesar.
     * @return "caesar"
     */
    public String getAlgName(){
       
        return "caesar"; 
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
        
        int x = 18;

        //Checks error in key.
        for(int i = 0; i < key.length; i++){
            if(((int)key[i]>=42)&&((int)key[i]<=122)){
                x = x + (int)key[i] - 42;
            }
            else{
                String error = "Error " + key[i] + " not allowed in key";
                throw new IndexOutOfBoundsException(error);
            }
        }

        caesarKey = x%81 + 42; 
    }

    /**
     * This methods error checks the plain message then encrypts it with the 
     * caesar method.
     * @param plain The message we are going to encrypt.
     * @return The encrypted message.
     * @throws IndexOutOfBoundsException throws error message if
     * a certian character in the plaintext is outside the ASCII values
     * 42 to 122. 
     */
    public String encrypt(String plain)  throws  IndexOutOfBoundsException{
        
        String c = "";
        char x;
        
        //checks error in plain then encrypts.
        for(int i = 0; i < plain.length(); i++){
            if(((int)plain.charAt(i)>=42)&&((int)plain.charAt(i)<=122)){
                x = (char)(42+(caesarKey + (int)plain.charAt(i) - 84)%81);
                c = c + x;
            }
            else{
                throw new IndexOutOfBoundsException("Error "
                 + plain.charAt(i) + " not allowed in plaintext");
            }
        }

        return c;
    }

    /**
     * This method error checks the cipher or encrypted text
     * and then decrypts it using the caesar method.
     * @param cipher The encrypted message.
     * @return The plain message that was encrypted in the cipher.
     * @throws IndexOutOfBoundsException throws error message if
     * a certian character in the cipher is outside the ASCII values
     * 42 to 122. 
     */
    public String decrypt(String cipher) throws IndexOutOfBoundsException{
        
        String c = "";
        char x;
        
        //check error in cipher then decrypts.
        for(int i = 0; i < cipher.length(); i++){
            if(((int)cipher.charAt(i)>=42)&&((int)cipher.charAt(i)<=122)){
                x = (char)(42+((cipher.charAt(i)-42)+(81+42-caesarKey))%81);
                c = c +x;
            }
            else{
                throw new IndexOutOfBoundsException("Error " + 
                cipher.charAt(i) + " not allowed in cybertext");                
            }
        }

        return c;
    }
  }