
package homework7;

import java.util.ArrayList;

public class _15210931_Ğ¤ÔÌÃ÷_7_CompositeStrategy implements _15210931_Ğ¤ÔÌÃ÷_7_IPricingStrategy
{
	private String index;
	private String name;
	private ArrayList<_15210931_Ğ¤ÔÌÃ÷_7_IPricingStrategy> strategies;

	public _15210931_Ğ¤ÔÌÃ÷_7_CompositeStrategy(
		String _index,
		String _name)
	{
		index = _index;
		name = _name;
		strategies = new ArrayList<_15210931_Ğ¤ÔÌÃ÷_7_IPricingStrategy>();
	}

	public void add (_15210931_Ğ¤ÔÌÃ÷_7_IPricingStrategy strategy) {
		strategies.add(strategy);
	}

	public String getIndex() {
		return index;
	}

	public String getName() {
		return name;
	}

	public int getType() {
		return 3;
	}

	public String getDescription() {
		if (strategies.size() == 0)
			return "";

		String temp = "";
		for (_15210931_Ğ¤ÔÌÃ÷_7_IPricingStrategy strategy : strategies)
			temp += strategy.getIndex() + "|";
		return temp.substring (0, temp.length() - 1);
	}

	public double getSubTotal(_15210931_Ğ¤ÔÌÃ÷_7_SaleLineItem _saleLineItem) {
		double minTotal = _saleLineItem.getOriginalTotal();
		for (_15210931_Ğ¤ÔÌÃ÷_7_IPricingStrategy strategy : strategies) {
			double subTotal = strategy.getSubTotal(_saleLineItem);
			if (subTotal < minTotal)
				minTotal = subTotal;
		}
		return minTotal;
	}
}