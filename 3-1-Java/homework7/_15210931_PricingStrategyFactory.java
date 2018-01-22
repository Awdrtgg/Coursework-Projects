
package homework7;

public class _15210931_肖蕴明_7_PricingStrategyFactory {
	private	_15210931_肖蕴明_7_StrategyCatalog catalog;
	private static _15210931_肖蕴明_7_PricingStrategyFactory instance;

	private _15210931_肖蕴明_7_PricingStrategyFactory() {}

	public static _15210931_肖蕴明_7_PricingStrategyFactory getInstance() {
		if (instance == null)
			instance = new _15210931_肖蕴明_7_PricingStrategyFactory();
		return instance;
	}

	public _15210931_肖蕴明_7_IPricingStrategy getPricingStrategy(int bookType) 
	throws Exception {
		if (catalog == null)
			throw new Exception("Invalid Book Type");

		_15210931_肖蕴明_7_IPricingStrategy ips = catalog.getStrategy(bookType);
		if (ips == null)
			return new _15210931_肖蕴明_7_NoDiscountStrategy();
		return ips;
	}

	public void setCatalog(_15210931_肖蕴明_7_StrategyCatalog _catalog) {
		catalog = _catalog;
	}
}