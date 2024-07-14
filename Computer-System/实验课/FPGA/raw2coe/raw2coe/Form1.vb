Imports System.IO

Public Class Form1

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Dim fName As String = ""
        If OpenFileDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then
            fName = OpenFileDialog1.FileName

            ' 读取二进制数据到数组
            Dim s() As Byte
            s = My.Computer.FileSystem.ReadAllBytes(fName)

            TextBox1.Text &= "; .COE file 十进制数据" & vbCrLf
            TextBox1.Text &= "; length: " & UBound(s) + 1 & ", Sampling frequency: 8000Hz" & vbCrLf
            TextBox1.Text &= "memory_initialization_radix=10;" & vbCrLf
            TextBox1.Text &= "memory_initialization_vector=" & vbCrLf

            ' StringBuilder可加速字符串的拼接过程
            Dim builder As New System.Text.StringBuilder
            For i As Integer = 0 To UBound(s)
                If (i = UBound(s)) Then
                    builder.Append(s(i) & ";")
                ElseIf (i > 0) And ((i + 1) Mod 10 = 0) Then
                    builder.Append(s(i) & "," & vbCrLf)
                Else
                    builder.Append(s(i) & ",")
                End If
            Next

            TextBox1.Text &= builder.ToString()
        End If
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        Dim coeFileName As String
        Dim fs As FileStream
        Dim w As StreamWriter

        If SaveFileDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then
            '1) 获取文件名
            coeFileName = SaveFileDialog1.FileName
            '2) 以写方式新建文件
            fs = New FileStream(coeFileName, FileMode.Create, FileAccess.Write)
            '3) 将文本写入fs中
            w = New StreamWriter(fs)
            '4) 将TextBox1文本写到文件中
            w.Write(TextBox1.Text)
            '5) 关闭w和fs
            w.Close()
        End If
    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        AboutMe.ShowDialog()
    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        end
    End Sub

End Class
