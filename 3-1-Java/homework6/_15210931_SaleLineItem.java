
package homework6;

public class _15210931_肖蕴明_6_SaleLineItem {
	private int copies;
	private _15210931_肖蕴明_6_ProductSpecification prodSpec;
	private _15210931_肖蕴明_6_IPricingStrategy strategy;

	public _15210931_肖蕴明_6_SaleLineItem(
		int _copies,
		_15210931_肖蕴明_6_ProductSpecification _prodSpec) 
		throws Exception
	{
		this.copies = _copies;
		this.prodSpec = _prodSpec;
		this.strategy = _15210931_肖蕴明_6_PricingStrategyFactory.getInstance().getPricingStrategy(_prodSpec.getType());
	}

	public int getCopies() {
		return this.copies;
	}

	public String getTitle() {
		return this.prodSpec.getTitle();
	}
	
	public _15210931_肖蕴明_6_ProductSpecification getProdSpec() {
		return this.prodSpec;
	}

	public double getPreTotal() {
		return (this.copies * this.prodSpec.getPrice());
	}

	public double getSubTotal() {
		return this.strategy.getSubTotal(this);
	}
}