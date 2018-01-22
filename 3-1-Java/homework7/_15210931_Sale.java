
package homework7;

import java.util.ArrayList;

public class _15210931_Ğ¤ÔÌÃ÷_7_Sale implements _15210931_Ğ¤ÔÌÃ÷_7_Subject {
	private ArrayList<_15210931_Ğ¤ÔÌÃ÷_7_Observer> observers;
	private ArrayList<_15210931_Ğ¤ÔÌÃ÷_7_SaleLineItem> items;

	public _15210931_Ğ¤ÔÌÃ÷_7_Sale() {
		this.observers = new ArrayList<_15210931_Ğ¤ÔÌÃ÷_7_Observer>();
		this.items = new ArrayList<_15210931_Ğ¤ÔÌÃ÷_7_SaleLineItem>();
	}

	public void addItem(_15210931_Ğ¤ÔÌÃ÷_7_SaleLineItem item) {
		this.items.add(item);
	}

	public ArrayList<_15210931_Ğ¤ÔÌÃ÷_7_SaleLineItem> getItems() {
		return this.items;
	}

	public double getTotal() {
		double temp = 0;
		for (_15210931_Ğ¤ÔÌÃ÷_7_SaleLineItem item : this.items)
			temp += item.getSubTotal();
		return temp; 
	}

	public void removeObserver(_15210931_Ğ¤ÔÌÃ÷_7_Observer observer) {
		this.observers.remove(observer);
	}

	public void registerObserver(_15210931_Ğ¤ÔÌÃ÷_7_Observer observer) {
		this.observers.add(observer);
	}
	
	public void notifyObservers() {
		for (_15210931_Ğ¤ÔÌÃ÷_7_Observer observer : this.observers)
			observer.update(this);
	}
}