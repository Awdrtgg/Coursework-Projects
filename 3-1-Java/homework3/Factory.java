package homework3;
import java.util.*;

public class Factory {
	private HashMap employees;
	
	public Factory() {
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
				this.employees.put(socialSecurityNumber, new SalaridEmployee(firstName, lastName, socialSecurityNumber, weeklySalary));
			}
			if(i == 1){
				System.out.print("请输入员工时薪：");
				double wage = in.nextDouble();
				System.out.print("请输入员工工作时长：");
				double hours = in.nextDouble();
				this.employees.put(socialSecurityNumber, new HourlyEmployee(firstName, lastName, socialSecurityNumber, wage, hours));
			}
			if(i == 2){
				System.out.print("请输入员工销售金额：");
				double grossSales = in.nextDouble();
				System.out.print("请输入员工销售提成：");
				double commisionRate = in.nextDouble();
				this.employees.put(socialSecurityNumber, new HourlyEmployee(firstName, lastName, socialSecurityNumber, grossSales, commisionRate));
			}
			if(i == 3){
				System.out.print("请输入员工基础薪水：");
				double baseSalary = in.nextDouble();
				System.out.print("请输入员工销售金额：");
				double grossSales = in.nextDouble();
				System.out.print("请输入员工销售提成：");
				double commisionRate = in.nextDouble();
				this.employees.put(socialSecurityNumber, new BasePlusCommisionEmployee(firstName, lastName, socialSecurityNumber, baseSalary, grossSales, commisionRate));
			}
		}
		this.printEmployees();
	}
	
	public Employee getEmployee(String empSecNum) {
		return (Employee)this.employees.get(empSecNum);
	}
	
	public Employee deleteEmployee(String empSecNum) {
		return (Employee)this.employees.remove(empSecNum);
	}
	
	public Employee addEmployee(Employee stu) {
		return (Employee)this.employees.put(stu.getSocialSecurityNumber(), stu);
	}
	
	public Employee addEmployee(BasePlusCommisionEmployee stu) {
		return (Employee)this.employees.put(stu.getSocialSecurityNumber(), stu);
	}
	
	public Employee addEmployee(CommisionEmployee stu) {
		return (Employee)this.employees.put(stu.getSocialSecurityNumber(), stu);
	}
	
	public Employee updateEmployee(String empSecNum, Employee emp) {
		Employee e = (Employee)this.employees.get(empSecNum);
		if (e == null) {
			return null;
		}
		else {
			return (Employee)this.employees.put(empSecNum, emp);
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
			sum += ((Employee)e.getValue()).earning();
		}
		return (sum / this.employees.size());
	}
}
