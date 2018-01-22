
package homework6;

import java.util.ArrayList;

public class _15210931_Ğ¤ÔÌÃ÷_6_Sale {
	private ArrayList<_15210931_Ğ¤ÔÌÃ÷_6_SaleLineItem> items;

	public _15210931_Ğ¤ÔÌÃ÷_6_Sale() {
		this.items = new ArrayList<_15210931_Ğ¤ÔÌÃ÷_6_SaleLineItem>();
	}

	public void addItem(_15210931_Ğ¤ÔÌÃ÷_6_SaleLineItem item) {
		this.items.add(item);
	}

	public ArrayList<_15210931_Ğ¤ÔÌÃ÷_6_SaleLineItem> getItems() {
		return this.items;
	}

	public double getTotal() {
		double temp = 0;
		for (_15210931_Ğ¤ÔÌÃ÷_6_SaleLineItem item : this.items)
			temp += item.getSubTotal();
		return temp; 
	}
}