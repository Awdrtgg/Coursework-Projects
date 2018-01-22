
package homework6;

public class _15210931_Ğ¤ÔÌÃ÷_6_NoDiscountStrategy implements _15210931_Ğ¤ÔÌÃ÷_6_IPricingStrategy {
	public String getIndex() {
		return null;
	}

	public String getName() {
		return null;
	}

	public int getType() {
		return 0;
	}

	public String getDescription() {
		return null;
	}

	public double getSubTotal(_15210931_Ğ¤ÔÌÃ÷_6_SaleLineItem _saleLineItem) {
		return (_saleLineItem.getCopies()
				* _saleLineItem.getProdSpec().getPrice());
	}
}