#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QRegExp>
#include <QDataStream>
#include <QCryptographicHash>

namespace KCloud{

	class UserManager;

    class User : public QObject{
        Q_OBJECT

		friend class UserManager;
		friend QDataStream &operator<<(QDataStream &out, const User &tmp);
		friend QDataStream &operator>>(QDataStream &inp, User &tmp);

		public:

			enum PwdMode{
				Encrypt,
				NotEncrypt
			};
			static	bool	checkMail(const QString &mail);			//lancia eccezioni

			explicit	User(QObject *parent = 0);
						User(const QString &mail, const QString &password, PwdMode mode = Encrypt, QObject *parent = 0);

				bool	isLogged() const;
				void	clear();
				void	setLogged();
				void	setUnLogged();
				void	setEmail(const QString &email);
				void	setHash(const QString &password, PwdMode mode = Encrypt);
				qint64	getSpace() const;
				QString getEmail() const;
				QString getHash() const;

		signals:

		public slots:

		private:
			bool        m_state;
			qint64      m_space;
			QString     m_email;
			QString     m_hash;
    };

	QDataStream &operator<<(QDataStream &out, const User &tmp);
	QDataStream &operator>>(QDataStream &inp, User &tmp);

}

#endif // USER_H