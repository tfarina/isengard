public class Stats {
    /**
     * Returns the minimum value in the specified array.
     */
    public static double Min(double[] a) {
        double min = Double.POSITIVE_INFINITY;

        for (int i = 0; i < a.length; i++) {
            if (Double.isNaN(a[i])) {
                return Double.NaN;
	    }

            if (a[i] < min) {
                min = a[i];
	    }
	}

	return min;
    }

    /**
     * Returns the maximum value in the specified array.
     */
    public static double Max(double[] a) {
        double max = Double.NEGATIVE_INFINITY;

        for (int i = 0; i < a.length; i++) {
            if (Double.isNaN(a[i])) {
                return Double.NaN;
	    }

            if (a[i] > max) {
                max = a[i];
	    }
	}

	return max;
    }

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

    /**
     * Returns the sample standard deviation in the specified array.
     */
    public static double StandardDeviation(double[] a) {
        return Math.sqrt(Variance(a));
    }

    public static void main(String[] args) {
        double[] a = { 2.4, 2.3, 2.7, 2.6 };

	System.out.printf("Min: %10.2f\n", Min(a));
	System.out.printf("Max: %10.2f\n", Max(a));
	System.out.printf("Mean: %10.2f\n", Mean(a));
	System.out.printf("Variance: %10.2f\n", Variance(a));
	System.out.printf("Standard deviation: %10.2f\n", StandardDeviation(a));
    }
}
