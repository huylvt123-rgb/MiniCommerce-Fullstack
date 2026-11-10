# MiniCommerce Full-stack — 7 Lab

Dự án chuyển từ ZIP offline sang Node.js + Express + PostgreSQL. Giao diện và các ví dụ cấu trúc dữ liệu/thuật toán vẫn nằm trong `public/index.html`. File C++ gốc ở `legacy/` chỉ để tham khảo, **không** chạy trên server.

## 1. Chạy trên máy tính

1. Cài Node.js 20+ và PostgreSQL 15+.
2. Tạo database `minicommerce` trong PostgreSQL (ví dụ `createdb minicommerce` hoặc dùng pgAdmin).
3. Sao chép `.env.example` thành `.env`, sửa `DATABASE_URL`, `ADMIN_USERNAME`, `ADMIN_PASSWORD` thành giá trị của bạn. Không công khai `.env`.
4. Mở Terminal/PowerShell trong thư mục dự án, chạy `npm install` và `npm start`.
5. Truy cập `http://localhost:3000`. Bảng dữ liệu và 12 sản phẩm mẫu được tạo khi khởi động lần đầu.
6. Chọn Admin và đăng nhập bằng tài khoản đặt trong `.env`. User chọn **Đăng ký ngay**, tạo tài khoản rồi đăng nhập.

Không mở file `index.html` trực tiếp bằng `file://` vì bản này cần API từ server.

## 2. Đăng lên Internet bằng Render + PostgreSQL

1. Tạo tài khoản GitHub và Render. Đưa thư mục dự án lên một **repository private** trên GitHub (không đưa `.env` lên).
2. Trong Render tạo **Postgres database**, chọn vùng gần dịch vụ web và lấy **Internal Database URL** (nếu cùng vùng và Render hỗ trợ). Kiểm tra chính sách giá, dung lượng, thời gian lưu trữ và sao lưu của gói bạn chọn.
3. Trong Render tạo **Web Service**, kết nối repo GitHub; Runtime: Node; Build Command: `npm install`; Start Command: `npm start`.
4. Trong **Environment** của Web Service đặt `DATABASE_URL` bằng URL Postgres, `NODE_ENV=production`, `ADMIN_USERNAME` và `ADMIN_PASSWORD` mạnh, riêng tư (ít nhất 12 ký tự). Render tự đặt `PORT` trong nhiều cấu hình; ứng dụng đọc biến này. Nếu nhà cung cấp yêu cầu TLS với CA hợp lệ, cấu hình `PGSSL=true`.
5. Deploy. Khi thành công, Render cung cấp một URL HTTPS dạng `https://ten-website.onrender.com`. Kiểm tra `https://ten-website.onrender.com/api/health` trả về `{"ok":true}`.
6. Mở URL ở hai thiết bị khác nhau: đăng ký User, đăng nhập, thêm giỏ, đặt hàng; đăng nhập Admin để xem và xử lý đơn. Bạn có thể cấu hình tên miền riêng trong phần Custom Domains của Render.

## 3. Bảo mật và lưu ý trước khi kinh doanh thật

- Mật khẩu băm bcrypt, session token ngẫu nhiên lưu hash trong PostgreSQL, cookie HttpOnly/SameSite/Secure khi production, kiểm tra vai trò ở server, giới hạn tần suất đăng nhập/đăng ký, truy vấn SQL tham số hóa, đặt hàng trong transaction có khóa hàng tồn kho.
- **Không có** thanh toán online, xác minh email, khôi phục mật khẩu, chống bot nâng cao, quản lý vận chuyển, quản lý thông tin người mua, sao lưu tự động, chính sách riêng tư hoặc hệ thống giám sát production.
- Bản này phù hợp **MVP thử nghiệm công khai**, chưa nên nhận tiền thật hoặc lưu thông tin cá nhân nhạy cảm. Trước khi mở kinh doanh, cần kiểm thử bảo mật, backup và hoàn thiện các tính năng trên.
- Đổi `ADMIN_PASSWORD` trong `.env` sau khi tài khoản admin đã tạo **không tự đổi mật khẩu trong DB**. Không dùng tên đăng nhập admin phổ biến nếu triển khai công khai. Nếu cần thay đổi, dùng quy trình reset mật khẩu có xác thực quản trị.
- `legacy/` chứa mã offline gốc và **có thể chứa thông tin đăng nhập demo**; không dùng những tài khoản đó trên website mới.
- Các thuật toán benchmark chạy phía trình duyệt; dữ liệu kho, giỏ hàng, đơn hàng mới được lưu ở server. Lịch sử tìm kiếm và Stack hoàn tác vẫn chỉ nằm trong phiên trình duyệt.
- Thông báo sản phẩm mới trong bản online hiện chưa đồng bộ giữa các Admin; thông báo số đơn đang chờ được lấy từ cơ sở dữ liệu mỗi khi tải lại trạng thái.
