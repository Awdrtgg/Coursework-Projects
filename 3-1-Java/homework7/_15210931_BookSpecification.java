
package homework7;

public class _15210931_肖蕴明_7_BookSpecification {
	private String isbn;
	private double price;
	private String title;
	private int type;

	public _15210931_肖蕴明_7_BookSpecification(
		String _isbn,
		String _title,
		double _price,
		int _type)
	{
		this.isbn = _isbn;
		this.title = _title;
		this.price = _price;
		this.type = _type;
	}

	public String getISBN() {
		return this.isbn;
	}

	public double getPrice() {
		return this.price;
	}

	public String getTitle() {
		return this.title;
	}

	public int getType() {
		return this.type;
	}
}