public class Percentage {
    public static int PercentageChange(int oldValue, int newValue) {
	int change = newValue - oldValue;

	return (change / oldValue) * 100;
    }

    public static void main(String[] args) {
	int change = PercentageChange(2, 10);

	System.out.println(change + "%");
    }
}
