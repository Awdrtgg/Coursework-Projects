package homework3;
import javax.script.ScriptContext;
import java.util.*;

public class Test3 {
	public static void main(String[] args){
		Object [] employee = new Object[10];
		Scanner in = new Scanner(System.in);
		Random rand =new Random(15);
		int i;
		for(int j = 0;j <= 2;j++){
			i=rand.nextInt(4);
			System.out.println(String.valueOf(j + 1)) ;
			System.out.print("请输入姓：");
			String lastName = in.next();
			System.out.print("请输入名字：");
			String firstName = in.next();
			System.out.print("请输入社保账号：");
			String socialSecurityNumber = in.next();
			if(i == 0){
				System.out.print("请输入员工每周薪水：");
				double weeklySalary = in.nextDouble();
				employee[j] = new SalaridEmployee(firstName, lastName, socialSecurityNumber, weeklySalary);
			}
			if(i == 1){
				System.out.print("请输入员工时薪：");
				double wage = in.nextDouble();
				System.out.print("请输入员工工作时长：");
				double hours = in.nextDouble();
				employee[j] = new HourlyEmployee(firstName, lastName, socialSecurityNumber, wage, hours);
			}
			if(i == 2){
				System.out.print("请输入员工销售金额：");
				double grossSales = in.nextDouble();
				System.out.print("请输入员工销售提成：");
				double commisionRate = in.nextDouble();
				employee[j] = new HourlyEmployee(firstName, lastName, socialSecurityNumber, grossSales, commisionRate);
			}
			if(i == 3){
				System.out.print("请输入员工基础薪水：");
				double baseSalary = in.nextDouble();
				System.out.print("请输入员工销售金额：");
				double grossSales = in.nextDouble();
				System.out.print("请输入员工销售提成：");
				double commisionRate = in.nextDouble();
				employee[j] = new BasePlusCommisionEmployee(firstName, lastName, socialSecurityNumber, baseSalary, grossSales, commisionRate);
			}
		}
		for(int j = 0; j <= 2 ; j++){
			System.out.println(String.valueOf(j + 1));
			System.out.print(employee[j].toString());
		}	
	}
}
