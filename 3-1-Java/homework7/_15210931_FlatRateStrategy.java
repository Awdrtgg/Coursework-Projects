
package homework7;

public class _15210931_Ð¤ÔÌÃ÷_7_FlatRateStrategy implements _15210931_Ð¤ÔÌÃ÷_7_IPricingStrategy {
	private String index;
	private String name;
	private double discountPerBook;

	public _15210931_Ð¤ÔÌÃ÷_7_FlatRateStrategy(
		String _index,
		String _name,
		double _discountPerBook)
	{
		this.index = _index;
		this.name = _name;
		this.discountPerBook = _discountPerBook;
	}

	public String getIndex() {
		return this.index;
	}

	public String getName() {
		return this.name;
	}
 
	public int getType() {
		return 1;
	}

	public String getDescription() {
		return "" + discountPerBook;
	}

	public double getSubTotal(_15210931_Ð¤ÔÌÃ÷_7_SaleLineItem _saleLineItem) {
		double result = (_saleLineItem.getCopies() 
						 * (_saleLineItem.getProdSpec().getPrice() 
						 - this.discountPerBook));
		if (result < 0) 
			return 0;
		return result;
	}
}