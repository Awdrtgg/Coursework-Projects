
package homework7;

import java.util.HashMap;

public class _15210931_肖蕴明_7_StrategyCatalog {
	public static String[] typeStrings = new String[] {
		"单本优惠", "百分比优惠", "组合优惠", "无优惠" };

	HashMap<Integer, _15210931_肖蕴明_7_IPricingStrategy> strategies;

	public _15210931_肖蕴明_7_StrategyCatalog() {
		strategies = new HashMap<Integer, _15210931_肖蕴明_7_IPricingStrategy>();
	}

	public HashMap<Integer, _15210931_肖蕴明_7_IPricingStrategy> getStrategies() {
		return strategies;
	}

	public _15210931_肖蕴明_7_IPricingStrategy getStrategy(int category) {
		return strategies.get(category);
	}

	public void setStrategy(int category, _15210931_肖蕴明_7_IPricingStrategy strategy) {
		strategies.put(category, strategy);
	}

	public void removeStrategy(int category) {
		strategies.remove(category);
	}
}