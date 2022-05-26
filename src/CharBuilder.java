public class CharBuilder {
    public CharBuilder(int[] decimal_code) {
		String string = "";
		for(int i=0; i<decimal_code.length; i++) {
			string += String.valueOf(((char)decimal_code[i]));
		}
		System.out.println(string);
	}
}
