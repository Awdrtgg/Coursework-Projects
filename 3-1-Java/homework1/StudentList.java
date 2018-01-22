public class StudentList {
	Student[] list;
	int total;
	
	public StudentList(int length) {
		this.list = new Student[length];
		for (int i = 0; i < length; i++) {
			this.list[i] = null;
		}
		this.total = 0;
	}
	
	public boolean add(Student stu) {
		if (this.total < this.list.length) {
			this.list[this.total] = stu;
			this.total += 1;
		}
		return false;
	}
	
	public boolean remove(int no) {
		if (no < 0 || no >= this.total) {
			return false;
		}
		else {
			for (int i = no; i < this.total; i++) {
				this.list[i] = this.list[i+1];
			}
			this.total -= 1;
			return true;
		}
	}
	
	public boolean remove(Student number) {
		int target = -1;
		for (int i = 0; i < this.total; i++) {
			if (this.list[i].getNumber().equals(number.getNumber())) {
				target = i;
				break;
			}
		}
		if (target >= 0) {
			return this.remove(target);
		}
		return false;
	}
	
	public boolean isEmpty() {
		return (this.total == 0);
	}
	
	public Student getItem(int no) {
		if (no < 0 || no >= this.total){
			return null;
		}
		return this.list[no];
	}
	
	public Student getItem(Student number) {
		int target = -1;
		for (int i = 0; i < this.total; i++) {
			if (this.list[i].getNumber().equals(number.getNumber())) {
				target = i;
				break;
			}
		}
		if (target >= 0) {
			return this.getItem(target);
		}
		return null;
	}
	
	public int getTotal() {
		return this.total;
	}
	
	public void printInfo() {
		System.out.println("---目前有" + getTotal() + "个学生，信息为---");
		for (int i = 0; i < this.total; i++)
			System.out.print(this.list[i].toString());
	}
}
