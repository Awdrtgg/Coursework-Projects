
package homework7;

public interface _15210931_肖蕴明_7_IPricingStrategy {
	public String getIndex ();
	public String getName ();
	public int getType ();
	public String getDescription ();

	public double getSubTotal(_15210931_肖蕴明_7_SaleLineItem _saleLineItem);
}