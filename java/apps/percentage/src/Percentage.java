public class Percentage {
    public static int percentage_increase(int oldValue, int newValue) {
	int increase = newValue - oldValue;

	return (increase / oldValue) * 100;
    }

    public static void main(String[] args) {
	int increase = percentage_increase(2, 10);

	System.out.println(increase + "%");
    }
}
