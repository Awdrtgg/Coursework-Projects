
package homework6;

public class _15210931_肖蕴明_6_PercentageStrategy implements _15210931_肖蕴明_6_IPricingStrategy {
	private double discountPercentage;

	public _15210931_肖蕴明_6_PercentageStrategy(double _discountPercentage) 
	throws Exception {
		if (_discountPercentage < 0 || _discountPercentage > 100)
			throw new Exception ("The range of discount percentage should be 0 ~ 100");

		this.discountPercentage = _discountPercentage;
	}
	
	public double getSubTotal(_15210931_肖蕴明_6_SaleLineItem _saleLineItem) {
			return (_saleLineItem.getCopies() 
					* _saleLineItem.getProdSpec().getPrice()
					* (100 - this.discountPercentage) 
					/ 100.0);
	}
}