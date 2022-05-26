package Errors;

public class IllegalCharacterError extends Err {
	public IllegalCharacterError(String linestr, int pos, int linepos) { 
		System.err.println(linestr);
		for(int i=0; i<pos; i++) System.out.print(' ');
		System.err.println("^");
		Throw("IllegalCharError at line "+linepos);
	}
}

