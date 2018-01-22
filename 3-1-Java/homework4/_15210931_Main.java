package homework4;
import java.util.*;

public class _15210931_肖蕴明_4_Main {
	public static void main(String[] args) {
		_15210931_肖蕴明_4_Factory f = new _15210931_肖蕴明_4_Factory();
		f.initEmpoyees();
		
		Scanner sc = new Scanner(System.in);
		int state = 0;
		int temp_int;
		String temp_str;
		double temp_d;
		_15210931_肖蕴明_4_Employee temp_emp;
		while (true) {
			System.out.println();
			System.out.println("操作列表：");
			System.out.println("1. 查询员工");
			System.out.println("2. 更新员工");
			System.out.println("3. 删除员工");
			System.out.println("4. 打印全部员工信息");
			System.out.print("请输入您的操作：");
			temp_int = sc.nextInt();
			if (temp_int <= 8 && temp_int >= 0) {
				state = temp_int;
			}
			else {
				continue;
			}
			sc.nextLine();
			switch (state) {
			case 1:
				System.out.print("请输入员工社保号：");
				temp_str = sc.nextLine();
				temp_emp = f.getEmployee(temp_str);
				if (temp_emp == null)
					System.out.println("员工不存在，查询失败");
				else 
					System.out.print(temp_emp.toString());
				break;
			
			case 2:
				System.out.print("请输入员工社保号：");
				temp_str = sc.nextLine();
				temp_emp = f.getEmployee(temp_str);
				if (temp_emp == null)
					System.out.println("员工不存在，更新失败");
				else {
					System.out.println("操作列表：");
					System.out.println("1. 更新姓");
					System.out.println("2. 更新名");
					System.out.print("请输入您的操作：");
					temp_int = sc.nextInt();
					temp_str = sc.nextLine();
					if (temp_int == 1) {
						System.out.print("请输入新的姓：");
						temp_str = sc.nextLine();
						temp_emp.setLastName(temp_str);
					}
					else if (temp_int == 2) {
						System.out.print("请输入新的名字：");
						temp_str = sc.nextLine();
						temp_emp.setLastName(temp_str);
					}
					else
						System.out.println("输入错误");
				}
				break;
				
			case 3:
				System.out.print("请输入员工社保号：");
				temp_str = sc.nextLine();
				temp_emp = f.deleteEmployee(temp_str);
				if (temp_emp == null)
					System.out.println("员工不存在，删除失败");
				else {
					System.out.println("已删除下列员工：");
					System.out.print(temp_emp.toString());
				}
				break;
				
			case 4:
				f.printEmployees();
				break;
				
			default:
				break;
			}
		}
	}
}
