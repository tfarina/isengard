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

	// Find the maximum.
	double max = Double.NEGATIVE_INFINITY;
	for (int i = 0; i < n; i++) {
	    if (a[i] > max) {
		max = a[i];
	    }
	}
	System.out.println("max = " + max);

        // Compute the sum of all values in the specified array.
        double sum = 0.0;
	for (int i = 0; i < n; i++) {
	    sum += a[i];
	}
	System.out.println("sum = " + sum);

        // Compute the average of the array values.
	sum = 0.0;
	for (int i = 0; i < n; i++) {
	    sum += a[i];
	}
	System.out.println("average = " + sum / n);
    }
}
