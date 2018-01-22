package homework4;
import java.util.*;

public class _15210931_肖蕴明_4_Factory {
	private HashMap employees;
	
	public _15210931_肖蕴明_4_Factory() {
		this.employees = new HashMap();
	}
	
	public void initEmpoyees() {
		Scanner in = new Scanner(System.in);
		Random rand = new Random(10);
		int i;
		for(int j = 0;j <= 3;j++){
			i=rand.nextInt(4);
			System.out.println(String.valueOf(j + 1));
			System.out.print("请输入姓：");
			String lastName = in.next();
			System.out.print("请输入名字：");
			String firstName = in.next();
			System.out.print("请输入社保账号：");
			String socialSecurityNumber = in.next();
			if (socialSecurityNumber.equals("0000")) 
				break;
			
			if(i == 0){
				System.out.print("请输入员工每周薪水：");
				double weeklySalary = in.nextDouble();
				this.employees.put(socialSecurityNumber, new _15210931_肖蕴明_4_SalaridEmployee(firstName, lastName, socialSecurityNumber, weeklySalary));
			}
			if(i == 1){
				System.out.print("请输入员工时薪：");
				double wage = in.nextDouble();
				System.out.print("请输入员工工作时长：");
				double hours = in.nextDouble();
				this.employees.put(socialSecurityNumber, new _15210931_肖蕴明_4_HourlyEmployee(firstName, lastName, socialSecurityNumber, wage, hours));
			}
			if(i == 2){
				System.out.print("请输入员工销售金额：");
				double grossSales = in.nextDouble();
				System.out.print("请输入员工销售提成：");
				double commisionRate = in.nextDouble();
				this.employees.put(socialSecurityNumber, new _15210931_肖蕴明_4_HourlyEmployee(firstName, lastName, socialSecurityNumber, grossSales, commisionRate));
			}
			if(i == 3){
				System.out.print("请输入员工基础薪水：");
				double baseSalary = in.nextDouble();
				System.out.print("请输入员工销售金额：");
				double grossSales = in.nextDouble();
				System.out.print("请输入员工销售提成：");
				double commisionRate = in.nextDouble();
				this.employees.put(socialSecurityNumber, new _15210931_肖蕴明_4_BasePlusCommisionEmployee(firstName, lastName, socialSecurityNumber, baseSalary, grossSales, commisionRate));
			}
		}
		this.printEmployees();
	}
	
	public _15210931_肖蕴明_4_Employee getEmployee(String empSecNum) {
		return (_15210931_肖蕴明_4_Employee)this.employees.get(empSecNum);
	}
	
	public _15210931_肖蕴明_4_Employee deleteEmployee(String empSecNum) {
		return (_15210931_肖蕴明_4_Employee)this.employees.remove(empSecNum);
	}
	
	public _15210931_肖蕴明_4_Employee addEmployee(_15210931_肖蕴明_4_Employee stu) {
		return (_15210931_肖蕴明_4_Employee)this.employees.put(stu.getSocialSecurityNumber(), stu);
	}
	
	public _15210931_肖蕴明_4_Employee addEmployee(_15210931_肖蕴明_4_BasePlusCommisionEmployee stu) {
		return (_15210931_肖蕴明_4_Employee)this.employees.put(stu.getSocialSecurityNumber(), stu);
	}
	
	public _15210931_肖蕴明_4_Employee addEmployee(_15210931_肖蕴明_4_CommisionEmployee stu) {
		return (_15210931_肖蕴明_4_Employee)this.employees.put(stu.getSocialSecurityNumber(), stu);
	}
	
	public _15210931_肖蕴明_4_Employee updateEmployee(String empSecNum, _15210931_肖蕴明_4_Employee emp) {
		_15210931_肖蕴明_4_Employee e = (_15210931_肖蕴明_4_Employee)this.employees.get(empSecNum);
		if (e == null) {
			return null;
		}
		else {
			return (_15210931_肖蕴明_4_Employee)this.employees.put(empSecNum, emp);
		}
	}
	
	public void printEmployees() {
		System.out.println("以下为当前所有的员工信息：");
		Iterator j = this.employees.entrySet().iterator();
		while (j.hasNext()) {
			Map.Entry e = (Map.Entry)j.next();
			System.out.println(e.getKey());
			System.out.print(e.getValue());
		}
	}
	
	public boolean isEmpty() {
		return (this.employees.size() == 0);
	}
	
	public double averageSalary() {
		Iterator j = this.employees.entrySet().iterator();
		double sum = 0.0;
		while (j.hasNext()) {
			Map.Entry e = (Map.Entry)j.next();
			sum += ((_15210931_肖蕴明_4_Employee)e.getValue()).earning();
		}
		return (sum / this.employees.size());
	}
}
