
package homework7;

public class _15210931_肖蕴明_7_PercentageStrategy implements _15210931_肖蕴明_7_IPricingStrategy {
	private String index;
	private String name;
	private double discountPercentage;

	public _15210931_肖蕴明_7_PercentageStrategy(
		String _index,
		String _name,
		double _discountPercentage) 
	throws Exception {
		if (_discountPercentage < 0 || _discountPercentage > 100)
			throw new Exception ("The range of discount percentage should be 0 ~ 100");

		this.index = _index;
		this.name = _name;
		this.discountPercentage = _discountPercentage;
	}

	public String getIndex() {
		return this.index;
	}

	public String getName() {
		return this.name;
	}

	public int getType() {
		return 2;
	}

	public String getDescription() {
		return "" + this.discountPercentage;
	}
	
	public double getSubTotal(_15210931_肖蕴明_7_SaleLineItem _saleLineItem) {
		return (_saleLineItem.getCopies() 
				* _saleLineItem.getProdSpec().getPrice()
				* (100 - this.discountPercentage) 
				/ 100.0);
	}
}