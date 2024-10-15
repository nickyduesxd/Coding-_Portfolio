/**
 * The CaesarShift class extends the Caesar class and implements the 
 * Hasher interface.
 * It's purpose is to hash a password.
 * @author MIDN 3/C NICHOLAS ZAYFMAN
 */
public class CaesarShift extends Caesar implements Hasher{
    
    //Private field to store the password.
    private static char[] psswd;

    /**
     * This method returns the algorithm name: shift+caesar.
     * @return "shift+caesar"
     */
    public String getAlgName(){
        return "shift+caesar";
    }

    /**
     * This method shift+ hashes the password using the Caesar method.
     * @param initVect which is the initialization vector.
     * @throws IndexOutOfBoundsException the hash class throws.
     * @return the hashed message.
     */
    public String hash(String initVect) throws IndexOutOfBoundsException{

        //Local fields
        char c;
        int k;
        String first = "";
        String second = "";
        int z = 0;
        int maxcount = psswd.length/16;

        //If password length is not a multiple of 16, add one to the maxcount.
        if(psswd.length%16!=0){
            maxcount++;
        }
        
        for(int j = 0; j < maxcount; j++){
            
            /* 
             * Splits the password into groups for reruning the message to 
             * the Caesar Encryption function.
             */
            if(j!=maxcount-1){
                super.init(((new String(psswd)).substring
                (j*16, j*16 + 16)).toCharArray());
            }
            else{
                if(psswd.length%16==0){
                    super.init(((new String(psswd)).substring
                    (j*16, j*16 + 16)).toCharArray());
                }
                else{
                    super.init(((new String(psswd)).substring
                    (j*16, j*16 + psswd.length%16)).toCharArray());
                }
            }
            
            //Hashes each group.
            for(int i = 0; i < 16; i++){
                c = initVect.charAt(i);
                k = (int)c%16;
                first = initVect.substring(0, k);
                second = initVect.substring(k, 16);
                initVect = second + first;
            
                try{
                    initVect = super.encrypt(initVect);
                } catch(IndexOutOfBoundsException t){
                    throw t;
                }
            }
        }

        return initVect;
    }

    /**
     * This method sets the key in the CaesarShift class to the inputed key.
     * @param key which is the key or password
     * @throws IndexOutOfBoundsException copies format from Hasher interface.
     */
    public void init(char[] key) throws IndexOutOfBoundsException{ 
        psswd = key;
    }

}