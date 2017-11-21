public class Arrays {
    public static void main(String[] args) {
	int n = Integer.parseInt(args[0]);

	double[] a = new double[n];
	for (int i = 0; i < n; i++) {
	    a[i] = Math.random();
	}

	for (int i = 0; i < n; i++) {
	    System.out.println(a[i]);
	}
    }
}
