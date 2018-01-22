
package homework7;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Set;
import java.util.Map.Entry;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.*;

public class _15210931_Ф����_7_StrategiesUI extends JFrame {
	private _15210931_Ф����_7_Controller controller;
	private JPanel panel;

	private static final int RH = 40;
	
	public _15210931_Ф����_7_StrategiesUI(_15210931_Ф����_7_Controller _controller) {
		controller = _controller;
		initUI();
	}
	
	private void initUI() {
		update();

		setTitle("�ۿ۲���");
		setLocationRelativeTo(null);
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
	}

	private void update() {
		HashMap<Integer, _15210931_Ф����_7_IPricingStrategy> strategies = controller.getStrategies();
		int[] categoryInTable = new int[strategies.size()];
		String[] categoryStrings = controller.getCategoryStrings();
		String[] strategyTypeStrings = controller.getStrategyTypeStrings();
		
		// Clear previous canvas
		if (panel != null)
			this.remove(panel);

		panel = new JPanel();
		panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));

		// Table
		String[] columnNames = {"���Ա��",
								"��������",
								"��������",
								"����ͼ������",
								"�Ż���Ϣ"};
		String[][] tableData = new String[strategies.size()][5];

		int index = 0;
		for (Entry<Integer, _15210931_Ф����_7_IPricingStrategy> strategyPair : strategies.entrySet()) {
			Integer category = strategyPair.getKey();
			_15210931_Ф����_7_IPricingStrategy strategy = strategyPair.getValue();

			tableData[index] = new String[] {
				strategy.getIndex(),
				strategy.getName(),
				strategyTypeStrings[strategy.getType() - 1],
				categoryStrings[category - 1],
				strategy.getDescription()
			};
			categoryInTable[index] = category;
			index++;
		}

		JTable table = new JTable(tableData, columnNames) {
			public boolean isCellEditable(int row, int column) {
				return false;
			}
		};
		table.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		table.setRowHeight(RH);
		panel.add(new JScrollPane(table));
		
		// Left
		JPanel lPanel = new JPanel();
		lPanel.setLayout(new GridLayout(3, 2));

		// Index
		lPanel.add(new JLabel("���Ա��"));
		JTextField tIndex = new JTextField(10);
		lPanel.add(tIndex);

		// Name
		lPanel.add(new JLabel("��������"));
		JTextField tName = new JTextField(10);
		lPanel.add(tName);

		// Category
		lPanel.add(new JLabel("����ͼ������"));
		JComboBox<String> cCategory = new JComboBox<String>(categoryStrings);
		lPanel.add(cCategory);

		// Right Panel
		JPanel vPanel = new JPanel();
		vPanel.setLayout(new BoxLayout(vPanel, BoxLayout.Y_AXIS));
		
		// Upper Right
		JPanel vPanel1 = new JPanel();
		vPanel1.setLayout(new GridLayout(2, 2));

		// Count
		vPanel1.add(new JLabel("�ۿ۰ٷֱ�/ÿ���Żݶ��"));
		JTextField tCount = new JTextField(10);
		vPanel1.add(tCount);

		// Button Add FlatRate
		JButton bAddFlatRate = new JButton("���þ���ֵ����");
		bAddFlatRate.addActionListener((e) -> {
			try {
				_15210931_Ф����_7_IPricingStrategy strategy = new _15210931_Ф����_7_FlatRateStrategy(
						tIndex.getText(),
						tName.getText(),
						Double.parseDouble(tCount.getText())
					);
				controller.setStrategy(cCategory.getSelectedIndex() + 1, strategy);
				update();
			}
			catch(NumberFormatException ee) {
				_15210931_Ф����_7_ErrorDlg log = new _15210931_Ф����_7_ErrorDlg("����ֵΪ�Ƿ�����");
                log.setVisible(true);
			}
			catch(Exception ee) {
				System.out.println(ee.getMessage());
			}
		});
		vPanel1.add(bAddFlatRate);

		// Button Add Percentage
		JButton bAddPercentage = new JButton("���ðٷֱȲ���");
		bAddPercentage.addActionListener((e) -> {
			try {
				_15210931_Ф����_7_IPricingStrategy strategy = new _15210931_Ф����_7_PercentageStrategy(
						tIndex.getText(),
						tName.getText(),
						Double.parseDouble(tCount.getText())
					);
				controller.setStrategy(cCategory.getSelectedIndex() + 1, strategy);
				update();
			}
			catch(NumberFormatException ee) {
				_15210931_Ф����_7_ErrorDlg log = new _15210931_Ф����_7_ErrorDlg("�ٷֱ�Ϊ�Ƿ�����");
                log.setVisible(true);
			}
			catch(Exception ee) {
				System.out.println(ee.getMessage());
			}
		});
		vPanel1.add(bAddPercentage);
		vPanel.add(vPanel1);
		
		// Down Right
		// For Composite & Delete & Quit
		JPanel vPanel2 = new JPanel();
		vPanel2.setLayout(new GridLayout(2, 2));

		// Hint
		vPanel2.add(new JLabel("ʹ�� Ctrl/Shift ��ѡ"));
		
		// Button Add Composite
		JButton bAddComposite = new JButton("���ѡ�в���Ϊ�²���");
		bAddComposite.addActionListener((e) -> {
			_15210931_Ф����_7_CompositeStrategy strategy = new _15210931_Ф����_7_CompositeStrategy(
					tIndex.getText(),
					tName.getText()
				);

			int[] rowIndices = table.getSelectedRows();
			for (int i : rowIndices)
				strategy.add(strategies.get(categoryInTable[i]));

			if (rowIndices.length > 0) {
				controller.setStrategy(cCategory.getSelectedIndex(), strategy);
				update();
			}
		});
		vPanel2.add(bAddComposite);

		// Button Delete
		JButton bDelete = new JButton("ɾ��ѡ�в���");
		bDelete.addActionListener((e) -> {
			int[] rowIndices = table.getSelectedRows();
			for (int i : rowIndices)
				controller.deleteStrategy(categoryInTable[i]);

			if(rowIndices.length > 0)
				update();
		});
		vPanel2.add(bDelete);

		// Button Cancel
		JButton bCancel = new JButton("�뿪");
		bCancel.addActionListener((e) -> {
			this.dispose();
		});
		vPanel2.add(bCancel);

		vPanel.add(vPanel2);
		
		// Foot
		JPanel fpanel = new JPanel();
		fpanel.setLayout(new GridLayout(1, 2));

		fpanel.add(lPanel);
		fpanel.add(vPanel);
		panel.add(fpanel);
		
		// Add Panel
		this.add(panel);
		this.pack();
		
		// when click on any row, copy all the data to text fields
		table.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent e) {
				 int row =((JTable)e.getSource()).rowAtPoint(e.getPoint()); // get the row
				 tIndex.setText((String)table.getValueAt(row, 0));
				 tName.setText((String)table.getValueAt(row, 1));
				 tCount.setText((String)table.getValueAt(row, 4));
				 cCategory.setSelectedIndex(row);
			}
		});
	}
}