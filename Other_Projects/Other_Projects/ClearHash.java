/**
 * The ClearHash class extends the Clear class and implements the 
 * Hasher interface.
 * It's purpose is to hash a password.
 * @author MIDN 3/C NICHOLAS ZAYFMAN
 */
public class ClearHash extends Clear implements Hasher{

    /**
     * This method returns the algorithm name: clear.
     * @return "clear"
     */
    public String getAlgName(){

        return super.getAlgName();
    }

    /**
     * This method shift+ hashes the password using the clear method.
     * @param initVect which is the initialization vector.
     * @throws IndexOutOfBoundsException the Clear class throws.
     * @return the hashed message.
     */
    public String hash(String initVect) throws IndexOutOfBoundsException{
        
        initVect = super.getPsswd();

        for(int i = initVect.length(); i < 16; i++){
            initVect = initVect + "x";
        }

        return initVect;
    }

    /**
     * This method sets the key in the Clear class to the inputed key.
     * @param key which is the key or password
     * @throws IndexOutOfBoundsException the Clear class throws.
     */
    public void init(char[] key) throws IndexOutOfBoundsException{ 
        super.init(key);
    }
}