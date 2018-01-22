
public class StudentNumberException extends RuntimeException{
	public StudentNumberException() {
		super("学生格式不对，请重新输入");
	}
}
