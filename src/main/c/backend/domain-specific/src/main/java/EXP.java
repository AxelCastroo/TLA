import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.Stack;
import java.util.function.Function;

public class EXP<T extends Comparable<? super T>> extends Tree<T> {
    private Node<String> root;

    public EXP() {
    }

    private Node<String> constructTree(String expression) {
        Stack<Node<String>> stack = new Stack<>();
        for (String token : expression.split(" ")) {
            if (isOperator(token)) {
                Node<String> node = new Node<>(token);
                node.setRight(stack.pop());
                node.setLeft(stack.pop());
                stack.push(node);
            } else {
                stack.push(new Node<>(token));
            }
        }
        return stack.pop();
    }

    private boolean isOperator(String token) {
        return token.equals("+") || token.equals("-") || token.equals("*") || token.equals("/") || token.equals("^");
    }

    public double calculate(String expression) {
        String postfixExpression = infijaToPosfija(expression);
        return evaluate(postfixExpression);
    }

    private static final Map<String, Integer> mapping = new HashMap<String, Integer>() {
        {
            put("+", 0);
            put("-", 1);
            put("*", 2);
            put("/", 3);
            put("^", 4);
            put("(", 5);
            put(")", 6);
        }
    };

    private static final boolean[][] precedenceMatriz =
            {
                    {true, true, false, false, false, false, true},
                    {true, true, false, false, false, false, true},
                    {true, true, true, true, false, false, true},
                    {true, true, true, true, false, false, true},
                    {true, true, true, true, false, false, true},
                    {false, false, false, false, false, false, false}
            };

    static boolean getPrecedence(String last, String current) {
        Integer lastIndex;
        Integer currentIndex;
        if ((lastIndex = mapping.get(last)) == null)
            throw new RuntimeException(String.format("tope operator %s not found", last));
        if ((currentIndex = mapping.get(current)) == null)
            throw new RuntimeException(String.format("current operator %s not found", current));
        return precedenceMatriz[lastIndex][currentIndex];
    }

    public double evaluate(String postfixExpression) {
        Scanner lineScanner = new Scanner(postfixExpression).useDelimiter("\\s+");

        Stack<Double> stack = new Stack<>();

        while (lineScanner.hasNext()) {
            String token = lineScanner.next();
            if (token.matches("-?[0-9]+(\\.[0-9]+)?"))
                stack.push(Double.valueOf(token));
            else {
                if (stack.size() < 2)
                    throw new IllegalArgumentException("Input is not in postfix notation: Wrong amount/order of operands.");
                Double ans;
                Double num2 = stack.pop();
                Double num1 = stack.pop();
                if (token.matches("\\+"))
                    ans = num1 + num2;
                else if (token.matches("-"))
                    ans = num1 - num2;
                else if (token.matches("\\*"))
                    ans = num1 * num2;
                else if (token.matches("/")) {
                    if (num2 == 0)
                        throw new IllegalStateException("Can't divide by zero");
                    ans = num1 / num2;
                } else
                    ans = Math.pow(num1, num2);
                stack.push(ans);
            }
        }
        return stack.pop();
    }

    public String infijaToPosfija(String s) {
        Scanner lineScanner = new Scanner(s).useDelimiter("\\s+");
        Stack<String> stack = new Stack<>();
        StringBuilder sb = new StringBuilder();
        while (lineScanner.hasNext()) {
            String token = lineScanner.next();
            if (token.matches("-?[0-9]+(\\.[0-9]+)?")) {
                sb.append(token).append(' ');
            } else if (token.matches("[+*^/()-]")) {
                while (!stack.isEmpty() && getPrecedence(stack.peek(), token)) {
                    sb.append(stack.pop()).append(' ');
                }
                if (token.matches("\\)")) {
                    if (stack.isEmpty() || stack.peek().compareTo("(") != 0)
                        throw new IllegalStateException("Didn't open the brackets");
                    stack.pop();
                } else stack.push(token);
            } else throw new IllegalStateException("Illegal character");
        }
        while (!stack.isEmpty()) {
            if (stack.peek().compareTo("(") == 0)
                throw new IllegalStateException("Didn't close all opened brackets");
            sb.append(stack.pop()).append(' ');
        }
        sb.deleteCharAt(sb.length() - 1); // borro el ultimo espacio
        return sb.toString();
    }

    @Override
    void insert(T element) {

    }

    @Override
    void remove(T element) {

    }

    @Override
    <E extends Comparable<? super E>> Tree<E> reduce(Function<T, E> function) {
        return null;
    }

    @Override
    T max() {
        return null;
    }

    @Override
    T min() {
        return null;
    }
}