
package homework7;

public class _15210931_肖蕴明_7_SaleLineItem {
	private int copies;
	private _15210931_肖蕴明_7_BookSpecification prodSpec;
	private _15210931_肖蕴明_7_IPricingStrategy strategy;

	public _15210931_肖蕴明_7_SaleLineItem(
		int _copies,
		_15210931_肖蕴明_7_BookSpecification _prodSpec) 
	throws Exception
	{
		this.copies = _copies;
		this.prodSpec = _prodSpec;
		this.strategy = _15210931_肖蕴明_7_PricingStrategyFactory.getInstance().getPricingStrategy(_prodSpec.getType());
	}

	public int getCopies() {
		return this.copies;
	}

	public String getTitle() {
		return this.prodSpec.getTitle();
	}
	
	public _15210931_肖蕴明_7_BookSpecification getProdSpec() {
		return this.prodSpec;
	}

	public double getOriginalTotal() {
		return (this.copies * this.prodSpec.getPrice());
	}

	public double getSubTotal() {
		return this.strategy.getSubTotal(this);
	}
}