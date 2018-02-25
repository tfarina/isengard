public class Stats {
    public static double Sum(double[] a) {
        double sum = 0.0;

        for (int i = 0; i < a.length; i++) {
            sum += a[i];
	}

        return sum;
    }

    public static double Mean(double[] a) {
        if (a.length == 0) {
            return Double.NaN;
	}

        double sum = Sum(a);

        return sum / a.length;  
    }

    /**
     * Returns the sample variance in the specified array.
     */
    public static double Variance(double[] a) {
        if (a.length == 0) {
            return Double.NaN;
	}

        double avg = Mean(a);
        double sum = 0.0;

        for (int i = 0; i < a.length; i++) {
            sum += (a[i] - avg) * (a[i] - avg);
	}

        return sum / (a.length - 1);
    }

    public static void main(String[] args) {
        double[] a = { 2.4, 2.3, 2.7, 2.6 };

	System.out.printf("Mean: %10.2f\n", Mean(a));
	System.out.printf("Variance: %10.2f\n", Variance(a));
    }
}
