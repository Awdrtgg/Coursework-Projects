
package homework6;

public class _15210931_肖蕴明_6_PricingStrategyFactory {
	private static _15210931_肖蕴明_6_PricingStrategyFactory instance;

	private _15210931_肖蕴明_6_PricingStrategyFactory() {}

	public static _15210931_肖蕴明_6_PricingStrategyFactory getInstance() {
		if (instance == null)
			instance = new _15210931_肖蕴明_6_PricingStrategyFactory();
		return instance;
	}

	public _15210931_肖蕴明_6_IPricingStrategy getPricingStrategy(int bookType) 
	throws Exception {
		if (bookType == 1)
			return new _15210931_肖蕴明_6_PercentageStrategy(3);
		else if (bookType == 2)
			return new _15210931_肖蕴明_6_FlatRateStrategy(1);
		else if (bookType == 3)
			return new _15210931_肖蕴明_6_PercentageStrategy(7);
		else
			return new _15210931_肖蕴明_6_NoDiscountStrategy();
	}
}