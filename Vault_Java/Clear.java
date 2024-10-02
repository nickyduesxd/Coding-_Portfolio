/**
 * This is the Clear class that implements Encryptor.
 * Clear encrypts and decrypts a message with the clear method.
 * @author MIDN 3/C Nicholas Zayfman
 */
public class Clear implements Encryptor {

    //Private Field
    private static String psswd;
  
    /**
     * This method returns the algorithm name: clear.
     * @return "clear"
     */
    public String getAlgName(){
      
      return "clear";
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
      for(int i = 0; i < key.length; i++){
        if(key[i]>122 || key[i]<42){
            String error = "Error " + key[i] + " not allowed in key";
            throw new IndexOutOfBoundsException(error);
        }
      }

      psswd = new String(key);
     }

    /**
     * This methods error checks the plain message then encrypts it with the 
     * clear method.
     * @param plain The message we are going to encrypt.
     * @return The encrypted message.
     * @throws IndexOutOfBoundsException throws error message if
     * a certian character in the plaintext is outside the ASCII values
     * 42 to 122. 
     */
    public String encrypt(String plain) throws IndexOutOfBoundsException{ 

      //checks error in plain then encrypts.
      for(int i = 0; i < plain.length(); i++){
        if(plain.charAt(i)>122 || plain.charAt(i)<42){
          throw new IndexOutOfBoundsException("Error " + plain.charAt(i) +
          " not allowed in plaintext");
        }
      }

      return plain; 
    }

    /**
     * This method error checks the cipher or encrypted text
     * and then decrypts it using the clear method.
     * @param cipher The encrypted message.
     * @return The plain message that was encrypted in the cipher.
     * @throws IndexOutOfBoundsException throws error message if
     * a certian character in the cipher is outside the ASCII values
     * 42 to 122. 
     */
    public String decrypt(String cipher) throws IndexOutOfBoundsException{ 
      
      //Checks error in cipher then decrypts.
      for(int i = 0; i < cipher.length(); i++){
        if(cipher.charAt(i)>122 || cipher.charAt(i)<42){
          throw new IndexOutOfBoundsException("Error " + cipher.charAt(i) +
          " not allowed in cybertext");
        }
      }
       
      return cipher; 
    }

    /**
     * This method returns the key.
     * @return psswd
     */
    public String getPsswd(){

      return psswd;
    }
  }