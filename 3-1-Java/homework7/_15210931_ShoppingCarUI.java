
package homework7;

import java.util.ArrayList;
import java.awt.*;
import javax.swing.*;

public class _15210931_肖蕴明_7_ShoppingCarUI extends JFrame implements _15210931_肖蕴明_7_Observer {
	private JScrollPane spane;
	private JPanel panel;

	public _15210931_肖蕴明_7_ShoppingCarUI() {
		initUI();
	}

	private void initUI() {
		this.panel = new JPanel();
		setTitle("购物车");
		setLocationRelativeTo(null);
		setDefaultCloseOperation(HIDE_ON_CLOSE);
	}

	public void update(_15210931_肖蕴明_7_Sale sale) {
		ArrayList<_15210931_肖蕴明_7_SaleLineItem> items = sale.getItems();
		double total = sale.getTotal();

		// Clear previous canvas
		if (spane != null)
			this.remove(spane);
		
		this.panel = new JPanel();
		panel.setLayout(new GridLayout(items.size() + 2, 4));
		// Header
		panel.add(new JLabel("书名"));
		panel.add(new JLabel("数量"));
		panel.add(new JLabel("原价"));
		panel.add(new JLabel("折扣后价格"));
		
		// List
		for (_15210931_肖蕴明_7_SaleLineItem item : items) {
			panel.add(new JLabel(item.getTitle()));
			panel.add(new JLabel("" + item.getCopies()));
			panel.add(new JLabel("" + item.getOriginalTotal()));
			panel.add(new JLabel("" + item.getSubTotal()));
		}

		// Total Price
		panel.add(new JLabel(""));
		panel.add(new JLabel(""));
		panel.add(new JLabel("折扣"));
		panel.add(new JLabel("" + total));
		
		
		// Add Panel
		spane = new JScrollPane(panel);
		this.add(spane);
		this.pack();
	}
}