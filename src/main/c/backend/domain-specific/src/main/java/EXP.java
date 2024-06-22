import java.util.Stack;
import java.util.function.Function;

public class EXP <T extends Comparable<? super T>> extends Tree<T> {
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
        return token.equals("+") || token.equals("-") || token.equals("*") || token.equals("/");
    }

    public int calculate(String expression) {
        root = constructTree(expression);
        return evaluate(root);
    }

    private int evaluate(Node<String> node) {
        if (node == null) {
            return 0;
        }

        if (!isOperator(node.getData())) {
            return Integer.parseInt(node.getData());
        }

        int leftValue = evaluate(node.getLeft());
        int rightValue = evaluate(node.getRight());

        switch (node.getData()) {
            case "+":
                return leftValue + rightValue;
            case "-":
                return leftValue - rightValue;
            case "*":
                return leftValue * rightValue;
            case "/":
                return leftValue / rightValue;
            default:
                throw new IllegalArgumentException("Invalid operator: " + node.getData());
        }
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