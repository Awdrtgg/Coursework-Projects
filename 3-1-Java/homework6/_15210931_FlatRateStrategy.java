
package homework6;

public class _15210931_肖蕴明_6_FlatRateStrategy implements _15210931_肖蕴明_6_IPricingStrategy {
	private double discountPerBook;

	public _15210931_肖蕴明_6_FlatRateStrategy(double _discountPerBook) {
		this.discountPerBook = _discountPerBook;
	}
 
	public double getSubTotal(_15210931_肖蕴明_6_SaleLineItem _saleLineItem) {
		double result = (_saleLineItem.getCopies() 
						 * (_saleLineItem.getProdSpec().getPrice() 
						 - this.discountPerBook));
		if (result < 0) 
			return 0;
		return result;
	}
}