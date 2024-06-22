import java.util.function.Function;

public class AVL<T extends Comparable<? super T>> extends Tree<T> {

    @Override
    public void insert(T element) {
        root = insert(root, element);
    }

    @Override
    public void remove(T element) {
        root = deleteNode(root, element);
    }

    @Override
    public T max() {
        if (root == null) {
            return null;
        }
        return maxValueNode(root).getData();
    }

    @Override
    public T min() {
        if (root == null) {
            return null;
        }
        return minValueNode(root).getData();
    }

    @Override
    <E extends Comparable<? super E>> Tree<E> reduce(Function<T, E> function) {
        Tree<E> tree = new AVL<>();
        for (Node<T> element : this) {
            if (element.getData() != null) {
                tree.insert(function.apply(element.getData()));
            }
        }
        return tree;
    }

    private Node<T> rightRotate(Node<T> y) {
        Node<T> x = y.getLeft();
        Node<T> T2 = x.getRight();

        x.setRight(y);
        y.setLeft(T2);

        y.setParent(x);
        if (T2 != null) {
            T2.setParent(y);
        }

        y.setH(Math.max(heightFromNode(y.getLeft()), heightFromNode(y.getRight())) + 1);
        x.setH(Math.max(heightFromNode(x.getLeft()), heightFromNode(x.getRight())) + 1);

        return x;
    }

    private Node<T> leftRotate(Node<T> x) {
        Node<T> y = x.getRight();
        Node<T> T2 = y.getLeft();

        y.setLeft(x);
        x.setRight(T2);

        y.setParent(x.getParent());
        x.setParent(y);
        if (T2 != null) {
            T2.setParent(x);
        }

        x.setH(Math.max(heightFromNode(x.getLeft()), heightFromNode(x.getRight())) + 1);
        y.setH(Math.max(heightFromNode(y.getLeft()), heightFromNode(y.getRight())) + 1);

        return y;
    }

    private int getBalance(Node<T> N) {
        if (N == null) {
            return 0;
        }
        return heightFromNode(N.getLeft()) - heightFromNode(N.getRight());
    }

    private Node<T> insert(Node<T> node, T element) {
        if (node == null) {
            return new Node<>(element);
        }

        if (element.compareTo(node.getData()) < 0) {
            node.setLeft(insert(node.getLeft(), element));
            node.getLeft().setParent(node);
        } else if (element.compareTo(node.getData()) > 0) {
            node.setRight(insert(node.getRight(), element));
            node.getRight().setParent(node);
        } else {
            return node;
        }

        node.setH(1 + Math.max(heightFromNode(node.getLeft()), heightFromNode(node.getRight())));

        int balance = getBalance(node);

        if (balance > 1 && element.compareTo(node.getLeft().getData()) < 0) {
            return rightRotate(node);
        }

        if (balance < -1 && element.compareTo(node.getRight().getData()) > 0) {
            return leftRotate(node);
        }

        if (balance > 1 && element.compareTo(node.getLeft().getData()) > 0) {
            node.setLeft(leftRotate(node.getLeft()));
            return rightRotate(node);
        }

        if (balance < -1 && element.compareTo(node.getRight().getData()) < 0) {
            node.setRight(rightRotate(node.getRight()));
            return leftRotate(node);
        }

        return node;
    }

    private Node<T> minValueNode(Node<T> node) {
        Node<T> current = node;
        while (current.getLeft() != null) {
            current = current.getLeft();
        }
        return current;
    }

    private Node<T> maxValueNode(Node<T> node) {
        Node<T> current = node;
        while (current.getRight() != null) {
            current = current.getRight();
        }
        return current;
    }

    private Node<T> deleteNode(Node<T> root, T element) {
        if (root == null) {
            return null;
        }

        if (element.compareTo(root.getData()) < 0) {
            root.setLeft(deleteNode(root.getLeft(), element));
        } else if (element.compareTo(root.getData()) > 0) {
            root.setRight(deleteNode(root.getRight(), element));
        } else {
            if ((root.getLeft() == null) || (root.getRight() == null)) {
                Node<T> temp = null;
                if (temp == root.getLeft()) {
                    temp = root.getRight();
                } else {
                    temp = root.getLeft();
                }

                if (temp == null) {
                    temp = root;
                    root = null;
                } else {
                    root = temp;
                }
            } else {
                Node<T> temp = minValueNode(root.getRight());
                root.setData(temp.getData());
                root.setRight(deleteNode(root.getRight(), temp.getData()));
            }
        }

        if (root == null) {
            return root;
        }

        root.setH(Math.max(heightFromNode(root.getLeft()), heightFromNode(root.getRight())) + 1);

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root.getLeft()) >= 0) {
            return rightRotate(root);
        }

        if (balance > 1 && getBalance(root.getLeft()) < 0) {
            root.setLeft(leftRotate(root.getLeft()));
            return rightRotate(root);
        }

        if (balance < -1 && getBalance(root.getRight()) <= 0) {
            return leftRotate(root);
        }

        if (balance < -1 && getBalance(root.getRight()) > 0) {
            root.setRight(rightRotate(root.getRight()));
            return leftRotate(root);
        }

        return root;
    }
}
