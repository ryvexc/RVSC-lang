import java.util.ArrayList;
import java.util.Scanner;
import Errors.IllegalCharacterError;

class Parser {
    private String value;

    public Parser(String value) {
        this.value = value;
    }

    public int[] Parse() {
        ArrayList<Integer> decval = new ArrayList<Integer>();
        int current_value = 0;
        int current_line = 1;
        String separator = this.value;

        for(int x=0; x<separator.length(); x++) {
            if(separator.charAt(x) == '+') {
                current_value += 16;
            } else if(separator.charAt(x) == '-') {
                current_value -= 8;
            } else if(separator.charAt(x) == '>') {
                current_value += 2;
            } else if(separator.charAt(x) == '<') {
                current_value -= 1;
            } else if(separator.charAt(x) == '.') {
                decval.add(current_value);
                current_line++;
            } 
            else {
                new IllegalCharacterError(separator, x, current_line);
                break;
            }
        }
        int[] arr = new int[decval.size()];
        for(int i=0; i<arr.length; i++) arr[i] = decval.get(i);
        return arr;
    }
}

public class Main {
    private static final Scanner scanner = new Scanner(System.in);
    
    public static void main(String[] args) {
        String input = scanner.nextLine();
        new CharBuilder(new Parser(input).Parse());
    }
}