
package homework7;

import java.util.ArrayList;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class _15210931_肖蕴明_7_BuyUI extends JFrame {
	private _15210931_肖蕴明_7_Controller controller;
	private _15210931_肖蕴明_7_Sale sale;
	private _15210931_肖蕴明_7_ShoppingCarUI shoppingCarUI;

	public _15210931_肖蕴明_7_BuyUI(_15210931_肖蕴明_7_Controller _controller) {
		controller = _controller;
		sale = controller.getSale();

		initUI();

		//shoppingCartUI = new ShoppingCarUI(sale);
		shoppingCarUI = new _15210931_肖蕴明_7_ShoppingCarUI();
		sale.registerObserver(shoppingCarUI);
		shoppingCarUI.update(sale);
	}

	private void initUI() {
		ArrayList<_15210931_肖蕴明_7_BookSpecification> books = controller.getBooks();
		String[] categoryStrings = controller.getCategoryStrings();

		JPanel panel = new JPanel();
		panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));

		// Scroll Pane
		JPanel spanel = new JPanel();
		spanel.setLayout(new GridLayout(books.size() + 1, 4));
		spanel.add(new JLabel("ISBN"));
		spanel.add(new JLabel("书名"));
		spanel.add(new JLabel("单价"));
		spanel.add(new JLabel("类型"));
		for (_15210931_肖蕴明_7_BookSpecification book : books) {
			spanel.add(new JLabel(book.getISBN()));
			spanel.add(new JLabel(book.getTitle()));
			spanel.add(new JLabel("" + book.getPrice()));
			spanel.add(new JLabel(categoryStrings[book.getType() - 1]));
		}
		panel.add(new JScrollPane(spanel));

		// Footer
		JPanel fpanel = new JPanel();
		fpanel.setLayout(new GridLayout(3, 2));

		// Input
		fpanel.add(new JLabel("ISBN"));
		JTextField tISBN = new JTextField(10);
		fpanel.add(tISBN);

		fpanel.add(new JLabel("数量"));
		JTextField tCopies = new JTextField(5);
		fpanel.add(tCopies);

		// Buttons
		JButton bBuy = new JButton("购物");
		bBuy.addActionListener((e) -> {
			try {
				controller.buyBook(
					tISBN.getText(),
					Integer.parseInt(tCopies.getText())
				);
				shoppingCarUI.setVisible(true);
			}
			catch(NumberFormatException ee) {
				_15210931_肖蕴明_7_ErrorDlg log = new _15210931_肖蕴明_7_ErrorDlg("输入错误");
                log.setVisible(true);
			}
			catch(Exception ee) {
				System.out.println(ee.getMessage());
			}
		});
		JButton bShowCart = new JButton("购物车");
		bShowCart.addActionListener((e) -> {
			shoppingCarUI.setVisible(true);
		});
		fpanel.add(bBuy);
		fpanel.add(bShowCart);

		panel.add(fpanel);
		
		// Add Panel
		this.add(panel);
		this.pack();

		// Misc
		setTitle("购买图书");
		setLocationRelativeTo(null);
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);

		// Handle Close Window
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				disposeShoppingCart();
				super.windowClosing(e);
			}
		});
	}

	private void disposeShoppingCart() {
		sale.removeObserver(shoppingCarUI);
		shoppingCarUI.dispose();
	}
}