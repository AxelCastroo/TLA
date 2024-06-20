public class Node<T extends Comparable<T>> {
    private T data;
    private Node<T> left;
    private Node<T> right;
    private Node<T> parent;
    private boolean color;  

    // Constructor
    public Node(T data) {
        this.data = data;
        this.left = null;
        this.right = null;
        this.parent = null;
        this.color = true; // New nodes are red by default in Red-Black Tree
    }

    // Getters and setters
    public T getData() {
        return data;
    }

    public void setData(T data) {
        this.data = data;
    }

    public Node<T> getLeft() {
        return left;
    }

    public void setLeft(Node<T> left) {
        this.left = left;
    }

    public Node<T> getRight() {
        return right;
    }

    public void setRight(Node<T> right) {
        this.right = right;
    }

    public Node<T> getParent() {
        return parent;
    }

    public void setParent(Node<T> parent) {
        this.parent = parent;
    }

    public boolean getColor() {
        return color;
    }

    public void setColor(boolean color) {
        this.color = color;
    }

    public boolean isRed() {
        return color == true;
    }

    public boolean isBlack() {
        return color == false;
    }

    public void makeRed() {
        this.color = true;
    }

    public void makeBlack() {
        this.color = false;
    }

    public boolean isLeaf() {
        return (this.left == null && this.right == null);
    }
}
